package financial;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class Connect {
	private static final String user = "root";
	private static final String pass = "password";
	private static final String url = "jdbc:mysql://127.0.0.1:3306";
	private static Connection con;
	
	public Connection Conn() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			
			con = DriverManager.getConnection(url,user,pass);
		} catch (SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		return con;
	}
}
