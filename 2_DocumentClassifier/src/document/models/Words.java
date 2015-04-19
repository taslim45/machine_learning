package document.models;

public class Words {

	public static final String TABLE_NAME = "words";
	
	public static final String ID = "_id";
	long id;
	
	public static final String WORD = "word";
	String word;
	
	public static final String TOPIC_ID = "topic_id";
	long topic_id;
	
	public static final String DOCUMENT_ID = "document_id";
	long document_id;
	
	public Words() {}

	public Words(long id, String word, long topic_id, long document_id) {
		super();
		this.id = id;
		this.word = word;
		this.topic_id = topic_id;
		this.document_id = document_id;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	public String getWord() {
		return word;
	}

	public void setWord(String word) {
		this.word = word;
	}

	public long getTopic_id() {
		return topic_id;
	}

	public void setTopic_id(long topic_id) {
		this.topic_id = topic_id;
	}

	public long getDocument_id() {
		return document_id;
	}

	public void setDocument_id(long document_id) {
		this.document_id = document_id;
	}
	
}
