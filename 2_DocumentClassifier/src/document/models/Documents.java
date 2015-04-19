package document.models;

public class Documents {

	public static final String TABLE_NAME = "documents";
	
	public static final String ID = "_id";
	long id;
	
	public static final String TOPICS_ID = "topics_id";
	long topic_id;
	
	public Documents() {}
	public Documents(long id, long topic_id) {
		super();
		this.id = id;
		this.topic_id = topic_id;
	}
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
	
}
