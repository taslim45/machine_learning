package document.classifier;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import document.database.*;
import document.models.Documents;
import document.models.Topics;

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
		File file = new File("out.txt");
		
		try (BufferedReader br = new BufferedReader(new FileReader("training.data")))
		{
			if (!file.exists()) {
				file.createNewFile();
			}
			String sCurrentLine;
			c.setAutoCommit(false);
			boolean isTopic,isTitle,isLocation,isStory,blankAfterTitle,blankAfterStory;
			isTopic = isTitle = isLocation = isStory = blankAfterTitle = false;
			blankAfterStory = true;
			int countBlanks = 0;
			
			FileWriter fw = new FileWriter(file.getAbsoluteFile());
			BufferedWriter bw = new BufferedWriter(fw);
			while ((sCurrentLine = br.readLine()) != null) 
			{
				int len = sCurrentLine.length();
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
					String[] currLine = splitLine(sCurrentLine);
					countBlanks = 0;
					if(currLine.length == 1 && !isTopic && blankAfterStory)
					{
						bw.write(sCurrentLine + ", ");
						
						isTopic = true;
						blankAfterStory = blankAfterTitle = false;
						long topicid = -1;
						long documentid = -1;
						//stmt = c.createStatement();
						
						StringBuilder sqlBuilder = new StringBuilder();
						sqlBuilder.append("SELECT * FROM ").append(Topics.TABLE_NAME);
						sqlBuilder.append(" WHERE ").append(Topics.NAME).append("=?");
						String sql = sqlBuilder.toString();
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
						//System.out.println(wow);
					}
					else if(sCurrentLine.charAt(len - 1) == '-' && !isLocation)
					{
						isLocation = true;
					}
					else
					{
						if(!blankAfterTitle)
						{
							// still getting title
							isTitle = true;
						}
						else
						{
							// it must be story
							isStory = true;
						}
					}
				}
				if(!isTopic)
				{
					
				}
				else if(!isTitle)
				{
					isTitle = true;
				}
				else if(!isLocation)
				{
					isLocation = true;
				}
				else if(!isStory)
				{
					isStory = true;
				}
				/*
				stmt = c.createStatement();
				StringBuilder sqlBuilder = new StringBuilder();
				
				
				String sql = sqlBuilder.toString();
				stmt.executeUpdate(sql);
				stmt.close();*/
			}
			
			c.commit();
			bw.close();
			fw.close();
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
