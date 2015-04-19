package document.models;

public class Topics {
	
	public static final String TABLE_NAME = "topics";
	
	public static final String ID = "_id";
	long id;
	
	public static final String NAME = "name";
	String name;
	
	public Topics() {}
	public Topics(long id, String name) {
		super();
		this.id = id;
		this.name = name;
	}
	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
}
