package financial;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;

public class Data {
	public int checkData() throws IOException {
		int data = -1;
		boolean complete = false;
		
		// Check for existing data
		while (complete != true) {File file = new File("data.dat");
			if (file.exists()) {
				FileReader fread = new FileReader(file);
				data = fread.read();
				complete = true;
			} else {
				break;
			}
		}
		return data;
	}
}
