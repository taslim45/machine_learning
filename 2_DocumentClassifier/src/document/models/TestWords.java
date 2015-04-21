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
	
	public static final String IS_CHECKED = "is_checked";
	int is_checked;
	
	public static long insert(Connection con,String word,long topicid,long documentid)
	{
		StringBuilder queryBuilder = new StringBuilder();
		queryBuilder.append("INSERT INTO ").append(TABLE_NAME);
		queryBuilder.append(" (").append(WORD).append(")");
		queryBuilder.append("VALUES ").append("(?)");
	
		try {
			PreparedStatement preparedStatement = con.prepareStatement(queryBuilder.toString());
			queryBuilder.setLength(0);
			preparedStatement.setString(1, word);
			long res = preparedStatement .executeUpdate();
			return res;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
		return -1;
	}

	public static String getNextWord(Connection c)
	{
		String sql = "SELECT " + WORD + " FROM " + TABLE_NAME + " WHERE " + IS_CHECKED + "=? LIMIT 1";
		String word = "";
		PreparedStatement pstmt = null;
		try
		{
			pstmt = c.prepareStatement(sql);
			pstmt.setInt(1, 0);
			ResultSet rs = pstmt.executeQuery();
			while(rs.next())
			{
				word = rs.getString(1);
				break;
			}
			rs.close();
			pstmt.close();
		}
		catch(SQLException ex)
		{
			ex.printStackTrace();
		}
		return word;
	}
	
	public static int getFrequency(Connection c , String word)
	{
		int count = 0;
		String sql = "SELECT COUNT("+ WORD +") FROM " + TABLE_NAME + " WHERE " + WORD + "=?";
		PreparedStatement pstmt = null;
		try
		{
			pstmt = c.prepareStatement(sql);
			pstmt.setString(1, word);
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
	
	public static void markWordAsChecked(Connection c,String word)
	{
		String sql = "UPDATE " + TABLE_NAME + " SET " + IS_CHECKED + "=? WHERE " + WORD + "=? AND " + IS_CHECKED + "=?";
		PreparedStatement pstmt = null;
		try
		{
			int count = 1;
			while(count>0)
			{
				pstmt = c.prepareStatement(sql);
				pstmt.setInt(1, 1);
				pstmt.setString(2, word);
				pstmt.setInt(3, 0);
				count = pstmt.executeUpdate();
				pstmt.close();
			}
		}
		catch(SQLException ex)
		{
			ex.printStackTrace();
		}
	
	}
	
	public static ArrayList<String> getDistinctWords(Connection c)
	{
		String sql = "SELECT DISTINCT(" + WORD + ") FROM " + TABLE_NAME;
		ArrayList<String> words = new ArrayList<String>();
		PreparedStatement pstmt = null;
		try
		{
			pstmt = c.prepareStatement(sql);
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
