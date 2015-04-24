package document.models;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;

public class TestDocuments {

public static final String TABLE_NAME = "test_documents";
	
	public static final String ID = "_id";
	long id;
	
	public static final String TOPICS_ID = "topic_id";
	long topic_id;

	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public long getTopic_id() {
		return topic_id;
	}
	public void setTopic_id(long topic_id) {
		this.topic_id = topic_id;
	}
	
	public static ArrayList<Long> getAllDocuments(Connection c)
	{
		ArrayList<Long> documents = new ArrayList<Long>();
		try {
			Statement stmt = c.createStatement();
			String sql = "SELECT * FROM " + TABLE_NAME;
			ResultSet rs = stmt.executeQuery(sql);
			
			while(rs.next())
			{
				documents.add(rs.getLong(2));
			}
			//return documents;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return documents;
	}
}
