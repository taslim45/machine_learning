package document.classifier;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import document.database.DBConnection;
import document.models.Documents;
import document.models.Topics;
import document.models.Words;

public class PosteriorCalculation {

	static double[] topicsProbability;
	static int totalTopics,totalDocuments,totalTrainingWords,totalVocabulary;
	
	public static void init()
	{
		int index = 0;
		try
		{
			Statement stmt = null;
			Connection c = DBConnection.getConnection();
			String sql = "SELECT COUNT(*) FROM " + Topics.TABLE_NAME;
			stmt = c.createStatement();
			ResultSet rs = stmt.executeQuery(sql);
			while(rs.next())
			{
				index = rs.getInt(1);
				break;
			}
			totalTopics = index;
			System.out.println(""+index);
			topicsProbability = new double[index+2];
			rs.close();
			stmt.close();
			
			sql = "SELECT COUNT(*) FROM " + Documents.TABLE_NAME;
			stmt = c.createStatement();
			rs = stmt.executeQuery(sql);
			while(rs.next())
			{
				index = rs.getInt(1);
				break;
			}
			totalDocuments = index;
			System.out.println(totalDocuments+"");
			rs.close();
			stmt.close();
			
			sql = "SELECT COUNT(DISTINCT(" + Words.WORD + ")) FROM " + Words.TABLE_NAME;
			stmt = c.createStatement();
			rs = stmt.executeQuery(sql);
			while(rs.next())
			{
				index = rs.getInt(1);
				break;
			}
			totalVocabulary = index;
			System.out.println(totalVocabulary+"");
			rs.close();
			stmt.close();
			
			c.close();
		}
		catch(SQLException ex)
		{
			ex.printStackTrace();
		}
	}
	
	public static void calculatePosteriorTopicProbability()
	{
		PreparedStatement pstmt = null;
		Connection c = DBConnection.getConnection();
		int i,count=0;
		for(i = 1; i<=totalTopics; i++)
		{
			try
			{
				String sql = "SELECT COUNT(*) FROM " + Documents.TABLE_NAME + " WHERE " + Documents.TOPICS_ID + "=?";
				pstmt = c.prepareStatement(sql);
				pstmt.setInt(1, i);
				ResultSet rs = pstmt.executeQuery();
				while(rs.next())
				{
					count = rs.getInt(1);
					break;
				}
				rs.close();
				pstmt.close();
			}
			catch(SQLException ex)
			{
				ex.printStackTrace();
			}
			topicsProbability[i] = (double)count / totalDocuments;
		}
		try {
			c.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static double getTopicProbability(int topic)
	{
		return topicsProbability[topic];
	}

	public static int getTotalTopics() {
		return totalTopics;
	}

	public static int getTotalDocuments() {
		return totalDocuments;
	}

	public static int getTotalTrainingWords() {
		return totalTrainingWords;
	}

	public static int getTotalVocabulary() {
		return totalVocabulary;
	}
	
	
}
