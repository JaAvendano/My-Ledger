package financial;

public class MenuItem {
	private static String m_label = null;
	private static String m_selector = null;
	
	public MenuItem(String selector, String label) {
		m_selector = selector; m_label = label;
	}
	
	public String getLabel() {return m_label;}
	public String getSelector() {return m_selector;}
}
