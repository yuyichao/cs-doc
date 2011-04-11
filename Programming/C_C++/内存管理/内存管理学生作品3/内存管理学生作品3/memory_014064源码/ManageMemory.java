import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.util.*;
import javax.swing.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * 模拟动态分区分配算法
 */
public class ManageMemory {
	//for test
	static public MyDispatcher testDispatcher = Utility.initDispatcher;

	public static void main(String[] args) {
		//test2();

		MemoryFrame frame = new MemoryFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.show();
	}
	static void test2() {
		int addr1 = testDispatcher.alloc(100);
		int addr2 = testDispatcher.alloc(100);
		int addr3 = testDispatcher.alloc(100);
		int addr4 = testDispatcher.alloc(100);
		int addr5 = testDispatcher.alloc(100);
		int addr6 = testDispatcher.alloc(100);

		testDispatcher.free(addr1);
		testDispatcher.free(addr2);
		testDispatcher.free(addr3);
		testDispatcher.free(addr4);
	}

	/*
	static void test1() {
		System.out.println(dispatcher);
		//作业1申请130K
		int addr1 = dispatcher.alloc(130);
		//作业2申请60K
		int addr2 = dispatcher.alloc(60);
		//作业3申请100k
		int addr3 = dispatcher.alloc(100);
		//作业2释放60K
		dispatcher.free(addr2);
		//作业4申请200K
		int addr4 = dispatcher.alloc(200);
		//作业3释放100K
		dispatcher.free(addr3);
		//作业1释放130K
		dispatcher.free(addr1);
		//作业5申请140K
		int addr5 = dispatcher.alloc(140);
		//作业6申请60K
		int addr6 = dispatcher.alloc(60);
		//作业7申请50K
		int addr7 = dispatcher.alloc(50);
		//作业6释放60K
		dispatcher.free(addr6);
	
		int addr8 = dispatcher.alloc(260);
	
		dispatcher.free(addr7);
	
		addr8 = dispatcher.alloc(260);
	}*/
}

class MemoryFrame extends JFrame {
	public MemoryFrame() {
		setSize(Utility.WIDTH, Utility.HEIGHT);
		setTitle("Memory Manage(014064 刘俊晖)");

		MemoryPanel memoryPanel = new MemoryPanel();
		Container contentPane = getContentPane();
		contentPane.add(memoryPanel);
	}
}

class MemoryPanel extends JPanel {
	private int allocLen = -1;
	private int dispatchID = -1;
	private JTextField textAlloc;
	private JTextField textFree;
	private JLabel labelResult;
	private JButton firstButton;
	private JButton bestButton;
	private DisplayMemoryPanel displayPanel;

	//可选择的内存分配器
	private MyDispatcher dispatcher = Utility.initDispatcher;
	private ArrayList useList = dispatcher.getUseList();
	private ArrayList freeList = dispatcher.getFreeList();

	public MemoryPanel() {
		setLayout(new BorderLayout());

		add(getSelectPanel(), BorderLayout.NORTH);
		add(getInputPanel(), BorderLayout.SOUTH);

		displayPanel = new DisplayMemoryPanel();
		displayPanel.setBackground(Color.GRAY);
		add(displayPanel);
	}

	//得到选择面板，选择实现的算法
	private JPanel getSelectPanel() {
		JPanel panel = new JPanel();

		firstButton = addButton(panel, "First", new FirstAction());
		bestButton = addButton(panel, "Best", new BestAction());

		if (dispatcher.getClass() == FirstDispatcher.class) {
			firstButton.setBackground(Color.BLUE);
		} else {
			bestButton.setBackground(Color.BLUE);
		}

		return panel;
	}

	//得到输入面板，输入要分配的内存大小和释放的进程ID
	private JPanel getInputPanel() {
		JPanel panel = new JPanel();
		JLabel label1 = new JLabel("Alloc Size (k)");
		JLabel label2 = new JLabel("Free ID");
		JLabel labelNull = new JLabel("      ");
		labelResult = new JLabel("Idle");
		
		textAlloc = new JTextField("100");
		textFree = new JTextField("1");
		textAlloc.setColumns(5);
		textFree.setColumns(5);

		JPanel resultPanel = new JPanel();
		resultPanel.add(labelResult);
		resultPanel.setBackground(Color.GREEN);

		JPanel inputPanel = new JPanel();
		inputPanel.add(label1);
		inputPanel.add(textAlloc);
		addButton(inputPanel, "OK", new AllocAction());
		inputPanel.add(labelNull);
		inputPanel.add(label2);
		inputPanel.add(textFree);
		addButton(inputPanel, "OK", new FreeAction());

		panel.setLayout(new BorderLayout());
		panel.add(resultPanel, BorderLayout.NORTH);
		panel.add(inputPanel, BorderLayout.SOUTH);
		return panel;
	}

	//选择最先适配算法
	private class FirstAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			System.out.println("First");
			//按钮变化
			firstButton.setBackground(Color.BLUE);
			bestButton.setBackground(null);
			//分配器更新
			dispatcher = new FirstDispatcher();
			useList = dispatcher.getUseList();
			freeList = dispatcher.getFreeList();
			//ID计数器更新
			Utility.resetDispatchID();
			//显示更新
			displayPanel.repaint();
			textAlloc.setText("100");
			textFree.setText("1");
			labelResult.setText("Idle");
		}
	}

	//选择最优适配算法
	private class BestAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			System.out.println("Best");
			//按钮变化
			bestButton.setBackground(Color.BLUE);
			firstButton.setBackground(null);
			//分配器更新
			dispatcher = new BestDispatcher();
			useList = dispatcher.getUseList();
			freeList = dispatcher.getFreeList();
			//ID计数器更新
			Utility.resetDispatchID();
			//显示更新
			displayPanel.repaint();
			textAlloc.setText("100");
			textFree.setText("1");
			labelResult.setText("Idle");
		}
	}

	//确定分配
	private class AllocAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String strLen = textAlloc.getText();
			if (strLen.length() == 0)
				return;

			int len;
			textAlloc.setText("");

			try {
				if ((len = Integer.parseInt(strLen)) == 0) {
					return;
				}
				int dispatchID;
				if ((dispatchID = dispatcher.alloc(len)) != -1)
				{
				    labelResult.setText("Alloc " + len + "k for process " + dispatchID);
				}
				else 
				{
					labelResult.setText("Not enough memory for allocing " + len + "k");
				}
				displayPanel.repaint();
			} catch (NumberFormatException e) {
				labelResult.setText("Please input integer!");
			}
		}
	}

	//确定释放
	private class FreeAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String strID = textFree.getText();
			if (strID.length() == 0)
				return;
			int dispatchID;
			textFree.setText("");

			try {
				dispatchID = Integer.parseInt(strID);
				if ((dispatcher.free(dispatchID)) == true)
				{
					labelResult.setText("Free process " + dispatchID);
				}
				else 
				{
					labelResult.setText("process " + dispatchID + " doesn't exist!");
				}
				displayPanel.repaint();
			} catch (NumberFormatException e) {
				labelResult.setText("input integer!");
			}
		}
	}

	private JButton addButton(
		JPanel panel,
		String label,
		ActionListener listener) {
		JButton button = new JButton(label);
		button.addActionListener(listener);
		panel.add(button);
		return button;
	}

	/*
	 * 显示内存状况和空闲分区表的面板类
	 */
	class DisplayMemoryPanel extends JPanel {
		public DisplayMemoryPanel() {
		}

		public void paintComponent(Graphics g) {
			super.paintComponent(g);

			Graphics2D g2 = (Graphics2D) g;
			g2.setColor(Color.BLACK);
			Rectangle2D rect1 =
				new Rectangle2D.Double(
					Utility.SPACE,
					Utility.SPACE,
					Utility.MEMORYWIDTH,
					Utility.initMemory * Utility.KILO);
			g2.draw(rect1);
			Rectangle2D rect2 =
				new Rectangle2D.Double(
					Utility.MEMORYWIDTH + Utility.SPACE * 2,
					Utility.SPACE,
					Utility.MEMORYWIDTH,
					Utility.initMemory * Utility.KILO);
			g2.draw(rect2);

			paintMemory(g);
			paintFreeList(g);
		}

		public void paintMemory(Graphics g) {
			SpaceNode node;
			for (int i = 0; i < useList.size(); i++) {
				node = (SpaceNode) useList.get(i);
				paintNode(
					true,
					g,
					node.getAddrFrom(),
					node.getAddrFrom(),
					node.getLen(),
					node.getDispatchID(),
					Color.YELLOW);
			}
			for (int i = 0; i < freeList.size(); i++) {
				node = (SpaceNode) freeList.get(i);
				paintNode(
					true,
					g,
					node.getAddrFrom(),
					node.getAddrFrom(),
					node.getLen(),
					node.getDispatchID(),
					Color.WHITE);
			}
		}

		public void paintFreeList(Graphics g) {
			SpaceNode node;
			int addrFrom = 0;
			for (int i = 0; i < freeList.size(); i++) {
				node = (SpaceNode) freeList.get(i);
				paintNode(
					false,
					g,
					addrFrom,
					node.getAddrFrom(),
					node.getLen(),
					0,
					Color.WHITE);
				addrFrom += node.getLen();
			}
		}

		public void paintNode(boolean isLeft, //是否在画总得内存状况
		Graphics g,
			int drawAddrFrom,
			int actualAddrFrom,
			int len,
			int dispatchID,
			Color color) {
			double x, y, width, length;
			if (isLeft == true) {
				x = Utility.SPACE;
			} else {
				x = Utility.MEMORYWIDTH + Utility.SPACE * 2;
			}

			y = Utility.SPACE + drawAddrFrom * Utility.KILO;
			width = Utility.MEMORYWIDTH;
			length = len * Utility.KILO;

			Graphics2D g2 = (Graphics2D) g;
			Rectangle2D rect1 = new Rectangle2D.Double(x, y, width, length);
			g2.setColor(color);
			g2.fill(rect1);
			g2.setColor(Color.BLACK);
			g2.draw(rect1);

			String strDraw;
			if (dispatchID == 0) {
				strDraw = "[Free Node] ";
			} else {
				strDraw = "[Process " + dispatchID + "] ";
			}
			g2.drawString(
				strDraw + "Address: " + actualAddrFrom + "  Length: " + len,
				(int) x,
				(int) y + len / 3);
		}
	}
}
