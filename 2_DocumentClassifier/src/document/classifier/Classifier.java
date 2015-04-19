package document.classifier;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

import document.database.*;
import document.models.Topics;

public class Classifier {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		readTopics();
	}
	
	private static void readTopics()
	{
		Statement stmt = null;
		Connection c = DBConnection.getConnection();
		try (BufferedReader br = new BufferedReader(new FileReader("topics.data")))
		{
			String sCurrentLine;
			c.setAutoCommit(false);
			while ((sCurrentLine = br.readLine()) != null) {
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
}
