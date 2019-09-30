package financial;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

public class setup {

	boolean status;
	Connection con = null;
	Statement stmt = null;
	PreparedStatement pstmt = null;
	
	public boolean createSchema() {
		try {
			System.out.println("Creating DB...");
			
			stmt = con.createStatement();
			stmt.execute("DROP SCHEMA IF EXISTS my_ledger");
			//pstmt = con.prepareStatement("CREATE SCHEMA my_ledger");
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	public boolean createAccounts() {
		try {
			System.out.println("Creating DB...");
			pstmt = con.prepareStatement("CREATE TABLE accounts (id INT NOT NULL AUTO_INCREMENT, institution VARCHAR(100) NOT NULL, type INT NOT NULL, name VARCHAR(50) NOT NULL, active INT NOT NULL, balance DECIMAL(9,2) NOT NULL, PRIMARY KEY (id)) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8");
			status = pstmt.execute();
			if(status = false) {
				System.out.println("No table created");
			}
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public boolean createTransactions() {
		try {
			System.out.println("Creating DB...");
			pstmt = con.prepareStatement("CREATE TABLE transactions ("
					+ "id NOT NULL AUTO_INCREMENT,"
					+ "acc_id INT NOT NULL,"
					+ "status VARCHAR(1),"
					+ "day INT NOT NULL,"
					+ "month INT NOT NULL,"
					+ "year INT NOT NULL,"
					+ "name VARCHAR(50) NOT NULL,"
					+ "method INT NOT NULL,"
					+ "cat INT NOT NULL,"
					+ "amount DECIMAL(15,2) NOT NULL,"
					+ "balance DECIMAL(15,2) NOT NULL,"
					+ "PRIMARY KEY (id)"
					+ "ENGINE = InnoDB DEFAULT CHARACTER SET = utf8");
			status = pstmt.execute();
			if(status = false) {
				System.out.println("No table created");
			}
		} catch (SQLException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public void closeConn() {
		try {
			if (pstmt != null) {
				pstmt.close();
			} 
			if (con != null) {
				con.close();
			}
		} catch (SQLException e) {
			System.out.println("Connection not properly closed!");
		}
	}
}
