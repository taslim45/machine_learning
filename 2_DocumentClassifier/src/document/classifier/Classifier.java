package document.classifier;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
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
		try (BufferedReader br = new BufferedReader(new FileReader("training.data")))
		{
			String sCurrentLine;
			c.setAutoCommit(false);
			boolean isTopic,isTitle,isLocation,isStory;
			isTopic = isTitle = isLocation = isStory = false;
			while ((sCurrentLine = br.readLine()) != null) 
			{
				//System.out.println(sCurrentLine);
				if(sCurrentLine.isEmpty())
				{
					if(isStory) isTopic = isTitle = isLocation = isStory = false;
					continue;
				}
				if(!isTopic)
				{
					isTopic = true;
					long topicid = -1;
					long documentid = -1;
					stmt = c.createStatement();
					StringBuilder sqlBuilder = new StringBuilder();
					sqlBuilder.append("SELECT * FROM ").append(Topics.TABLE_NAME);
					sqlBuilder.append(" WHERE ").append(Topics.NAME).append("='").append(sCurrentLine).append("'");
					String sql = sqlBuilder.toString();
					ResultSet rs = stmt.executeQuery(sql);
					stmt.close();
					while(rs.next())
					{
						topicid = rs.getLong(Topics.ID);
						break;
					}
					
					stmt = c.createStatement();
					sqlBuilder.setLength(0);
					sqlBuilder.append("INSERT INTO ").append(Documents.TABLE_NAME);
					sqlBuilder.append(" (").append(Documents.TOPICS_ID).append(")");
					sqlBuilder.append(" VALUES ");
					sqlBuilder.append("(").append(topicid).append(")");
					sql = sqlBuilder.toString();
					
					long wow = stmt.executeUpdate(sql);
					stmt.close();
					System.out.println(wow);
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
			if(c!=null) {
				c.setAutoCommit(true); 
				c.close();
			}
		} catch (IOException | SQLException e) {
			e.printStackTrace();
		}
		System.out.println("Done");
	}
}
