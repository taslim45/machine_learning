package document.models;

public class Topics {
	
	public static String TABLE_NAME = "topics";
	
	public static String ID = "_id";
	public long id;
	
	public static String NAME = "name";
	public String name;
	
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
