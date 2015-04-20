package document.classifier;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import utils.WordUtils;

import document.database.*;
import document.models.Documents;
import document.models.Topics;
import document.models.Words;

public class Classifier {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		clearDatabase();
		readTopics();
		readTrainingData();
	}
	
	private static void clearDatabase()
	{
		Statement stmt = null;
		Connection c = DBConnection.getConnection();
		try {
			c.setAutoCommit(false);
			
			stmt = c.createStatement();
			String sql1 = "DELETE FROM " + Topics.TABLE_NAME;
			stmt.executeUpdate(sql1);
			stmt.close();
			stmt = c.createStatement();
			sql1 = "DELETE FROM " + Documents.TABLE_NAME;
			stmt.executeUpdate(sql1);
			stmt.close();
			c.commit();
			if(c!=null) 
			{
				c.setAutoCommit(true);
				c.close();
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	private static void readTopics()
	{
		Statement stmt = null;
		Connection c = DBConnection.getConnection();
		try (BufferedReader br = new BufferedReader(new FileReader("topics.data")))
		{
			String sCurrentLine;
			c.setAutoCommit(false);
			while ((sCurrentLine = br.readLine()) != null) 
			{
				//System.out.println(sCurrentLine);
				stmt = c.createStatement();
				StringBuilder sqlBuilder = new StringBuilder();
				sqlBuilder.append("INSERT INTO ");
				sqlBuilder.append(Topics.TABLE_NAME).append(" ");
				sqlBuilder.append("( ").append(Topics.NAME).append(" ) ");
				sqlBuilder.append("VALUES ");
				sqlBuilder.append("('").append(sCurrentLine).append("')");
				String sql = sqlBuilder.toString();
				stmt.executeUpdate(sql);
				stmt.close();
				sqlBuilder.setLength(0);
			}
			c.commit();
			if(c!=null) {
				c.setAutoCommit(true); 
				c.close();
			}
		} catch (IOException | SQLException e) {
			e.printStackTrace();
		}
		System.out.println("Done");
	}
	
	private static void readTrainingData()
	{
		Statement stmt = null;
		Connection c = DBConnection.getConnection();
		PreparedStatement pstmt = null;
		
		try (BufferedReader br = new BufferedReader(new FileReader("training.data")))
		{
			String sCurrentLine;
			c.setAutoCommit(false);
			boolean isTopic,isTitle,isLocation,isStory,blankAfterTitle,blankAfterStory;
			isTopic = isTitle = isLocation = isStory = blankAfterTitle = false;
			blankAfterStory = true;
			int countBlanks = 0;
			String[] currLine;
			long documentid = -1;
			long topicid = -1;
			int len;
			while ((sCurrentLine = br.readLine()) != null) 
			{
				len = sCurrentLine.length();
				
				//System.out.println(sCurrentLine);
				if(sCurrentLine.isEmpty())
				{
					countBlanks++;
					if(isStory || countBlanks >=2) 
					{
						isTopic = isTitle = isLocation = isStory = false;
					}
					if(countBlanks >= 2) blankAfterStory = true;
					if(isTitle) blankAfterTitle = true;
					continue;
				}
				else
				{
					currLine = splitLine(sCurrentLine);
					countBlanks = 0;
					if(currLine.length == 1 && !isTopic && blankAfterStory)
					{
						isTopic = true;
						blankAfterStory = blankAfterTitle = false;
						//stmt = c.createStatement();
						
						StringBuilder sqlBuilder = new StringBuilder();
						sqlBuilder.append("SELECT * FROM ").append(Topics.TABLE_NAME);
						sqlBuilder.append(" WHERE ").append(Topics.NAME).append("=?");
						String sql = sqlBuilder.toString();
						sqlBuilder.setLength(0);
						pstmt = c.prepareStatement(sql);
						pstmt.setString(1, sCurrentLine);
						ResultSet rs = pstmt.executeQuery();
						//stmt.close();
						while(rs.next())
						{
							topicid = rs.getLong(Topics.ID);
							break;
						}
						rs.close();
						pstmt.close();
						
						stmt = c.createStatement();
						sqlBuilder.setLength(0);
						sqlBuilder.append("INSERT INTO ").append(Documents.TABLE_NAME);
						sqlBuilder.append(" (").append(Documents.TOPICS_ID).append(")");
						sqlBuilder.append(" VALUES ");
						sqlBuilder.append("(").append(topicid).append(")");
						sql = sqlBuilder.toString();
						
						long wow = stmt.executeUpdate(sql);
						stmt.close();
						if(wow > 0)
						{
							stmt = c.createStatement();
							rs = stmt.executeQuery("SELECT last_insert_rowid()");
							while(rs.next())
							{
								documentid = rs.getLong(1);
								break;
							}
							rs.close();
							stmt.close();
						}
						//System.out.println(wow);
					}
					else if(sCurrentLine.charAt(len - 1) == '-' && !isLocation)
					{
						isLocation = true;
						for (String word : currLine) 
						{
							String modified = WordUtils.cleanWord(word.toLowerCase().trim());
							if(modified.isEmpty()) continue;
							Words.insert(c, modified, topicid, documentid);
						}
					}
					else
					{
						if(!blankAfterTitle)
						{
							// still getting title
							isTitle = true;
							for (String word : currLine) 
							{
								String modified = WordUtils.cleanWord(word.toLowerCase().trim());
								if(modified.isEmpty()) continue;
								//Words.insert(c, modified, topicid, documentid);
							}
						}
						else
						{
							// it must be story
							isStory = true;
							for (String word : currLine) 
							{
								String modified = WordUtils.cleanWord(word.toLowerCase().trim());
								if(modified.isEmpty()) continue;
								//Words.insert(c, modified, topicid, documentid);
							}
						}
					}
				}

			}
			
			c.commit();
			if(c!=null) {
				c.setAutoCommit(true); 
				c.close();
			}
		} catch (IOException | SQLException e) {
			e.printStackTrace();
		}
		System.out.println("Done");
	}
	
	private static String[] splitLine(String line)
	{
		String[] splitStr = line.split("\\s+");
		return splitStr;
	}
}
