package financial;

import java.util.Map;

public class Menu {
	private static String m_cmdExit = null;
	private static Map<String, MenuItem> m_menuItemList;
	
	public int addMenuItem(MenuItem pMenuItem) {
		int status = 0;
		
		if(pMenuItem == null) {
			status = -1;
			System.out.println("ERROR: Menu::addMenuItem cannot add menu item value with null pointer");
		} else {
			String selector = pMenuItem.getSelector();
			Map<String, MenuItem>
		}
		
		return status;
	};
	public String execute() {};
}
