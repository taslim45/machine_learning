package document.models;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import document.database.DBConnection;

public class TestWords {

public static final String TABLE_NAME = "test_words";
	
	public static final String ID = "_id";
	long id;
	
	public static final String WORD = "word";
	String word;
	
	public static final String TOPIC_ID = "topic_id";
	long topic_id;
	
	public static final String DOCUMENT_ID = "document_id";
	long document_id;
	
	
	public static long insert(Connection con,String word,long topicid,long documentid)
	{
		StringBuilder queryBuilder = new StringBuilder();
		queryBuilder.append("INSERT INTO ").append(TABLE_NAME);
		queryBuilder.append(" (").append(WORD).append(",");
		queryBuilder.append(TOPIC_ID).append(",").append(DOCUMENT_ID).append(") ");
		queryBuilder.append("VALUES ").append("(?,?,?)");
	
		try {
			PreparedStatement preparedStatement = con.prepareStatement(queryBuilder.toString());
			queryBuilder.setLength(0);
			preparedStatement.setString(1, word);
			preparedStatement.setLong(2, topicid);
			preparedStatement.setLong(3, documentid);
			long res = preparedStatement .executeUpdate();
			return res;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
		return -1;
	}
	
	public static int getFrequency(Connection c , String word, long documentid)
	{
		int count = 0;
		String sql = "SELECT COUNT("+ WORD +") FROM " + TABLE_NAME + " WHERE " + WORD + "=? AND " + DOCUMENT_ID + "=?";
		PreparedStatement pstmt = null;
		try
		{
			pstmt = c.prepareStatement(sql);
			pstmt.setString(1, word);
			pstmt.setLong(2, documentid);
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
		return count;
	}
	
	
	public static ArrayList<String> getDistinctWords(Connection c,long documentid)
	{
		String sql = "SELECT DISTINCT(" + WORD + ") FROM " + TABLE_NAME + " WHERE " + DOCUMENT_ID + "=?";
		ArrayList<String> words = new ArrayList<String>();
		PreparedStatement pstmt = null;
		try
		{
			pstmt = c.prepareStatement(sql);
			pstmt.setLong(1, documentid);
			ResultSet rs = pstmt.executeQuery();
			
			while(rs.next())
			{
				words.add(rs.getString(1));
			}
			rs.close();
			pstmt.close();
		}
		catch(SQLException ex)
		{
			ex.printStackTrace();
		}
		
		return words;
	}
}
