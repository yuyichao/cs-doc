import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;


//	 测试模拟内存分配的类
public class MemoryAllocTest {

	public MemoryAllocTest() {
		super();

	}
	
	//	主函数
	public static void main(String[] args) {
		//	主框架的实例化
		MemoryAllocFrame frame = new MemoryAllocFrame("MemoryAlloc");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		frame.setLocation(300, 10);
		frame.show();

	}
}

//	主框架的实现类
class MemoryAllocFrame extends JFrame {
	//	设置的窗口长和宽
	public static final int DEFAULT_WIDTH = 500;
	public static final int DEFAULT_HEIGHT = 730;
	// 	主框架类的构造函数
	public MemoryAllocFrame(String str) {
		super(str);
		setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		Container contentPane = getContentPane();
		//	主框架内的Panel类的实例化
		MemoryAllocPanel panel = new MemoryAllocPanel();
		contentPane.add(panel);
	}
}

//	负责显示所模拟的内存分配的柱状图的类
class GraphicPanel extends JPanel {
	//	柱状图的位置和 长宽
	public static final int RECT_WIDTH = 100;
	public static final int RECT_HEIGHT = 640;
	public static final int INIT_LOCA_X = 50;
	public static final int INIT_LOCA_Y = 30;

	public SortedMap allocatedMap = new TreeMap();
	public SortedMap allocatedStartMap = new TreeMap();

	public GraphicPanel() {

	}
	
	//	用于接收发给来的内存分配信息，接受后再显示
	public void setAllocatedMap(SortedMap map, SortedMap startMap) {
		allocatedMap = map;
		allocatedStartMap = startMap;

		Iterator it = allocatedStartMap.keySet().iterator();
		//		while(it.hasNext())
		{
			//			System.out.println(allocatedStartMap.get(it.next()));
		}
	}
	//	绘制柱状图，用来模拟内存的分配
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		
		//	画外框
		Rectangle2D rect =
			new Rectangle2D.Double(
				INIT_LOCA_X,
				INIT_LOCA_Y,
				RECT_WIDTH,
				RECT_HEIGHT);
		g2.draw(rect);
		//	画带有数字的标记
		for (int i = 0; i < 10; ++i) {
			String str = "" + i * 80 + "K";
			g2.drawLine(
				INIT_LOCA_X + RECT_WIDTH,
				INIT_LOCA_Y + i * 80,
				INIT_LOCA_X + RECT_WIDTH + 10,
				INIT_LOCA_Y + i * 80);
			g2.drawString(str, 170, i * 80 + INIT_LOCA_Y);
		}

		Iterator itAllocatedMap = allocatedMap.keySet().iterator();
		while (itAllocatedMap.hasNext()) {
			Double StartRect = (Double) itAllocatedMap.next();
			Double LengthOfRect = (Double) (allocatedMap.get(StartRect));

			double startRect = StartRect.doubleValue();
			double length = LengthOfRect.doubleValue();

			double d = INIT_LOCA_Y + startRect;
			
			//	画模拟的内存分配
			Rectangle2D allocRect =
				new Rectangle2D.Double(
					INIT_LOCA_X,
					INIT_LOCA_Y + startRect,
					RECT_WIDTH,
					length);

			g2.setPaint(Color.red);
			g2.fill(allocRect);		//	填充
			g2.setPaint(Color.black);
			g2.draw(allocRect);

			Iterator it = allocatedStartMap.keySet().iterator();
			//				while(it.hasNext())
			{
				//					System.out.println(allocatedStartMap.get(it.next()));
			}
			
			//	画标识进程ID的ProcessID
			if (length >= 10) {
				String str =
					""
						+ ((Integer) allocatedStartMap.get(StartRect)).intValue();
				g2.setPaint(Color.black);
				g2.drawString(
					str,
					INIT_LOCA_X + 45,
					(int) (INIT_LOCA_Y + startRect + length / 2 + 5));
			}
		}

	}
}

//	显示操作信息的类，用于提示反映每一步的操作结果
class DispPanel extends JPanel {
	private JTextArea textArea;
	private JScrollPane scrollPane;

	//	该类的构造函数
	public DispPanel() {

		setLayout(new GridLayout(2, 1));

		add(new JLabel(""));
		
		textArea = new JTextArea(3, 40); //3行40列
		textArea.setEditable(false);
		scrollPane = new JScrollPane(textArea);
		add(scrollPane);

	}

	//	画一些提示信息
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;

		String str1 = "内存管理之大作业";
		String str2 = "动态分区分配方式的模拟";
		String str3 = "1.  最先适配算法";
		String str4 = "2.  最佳适配算法";
		g2.drawString(str1, 50, 50);
		g2.drawString(str2, 35, 75);
		g2.drawString(str3, 50, 100);
		g2.drawString(str4, 50, 125);
	}
	
	//	更新文本区内容，显示每步的操作结果
	public void updateTextArea(String msg) {
		textArea.append(msg);
	}
	
	public void upDateDisp(
		SortedMap allocatedMap,
		SortedMap allocatedStartMap) {
		Iterator it = allocatedMap.keySet().iterator();

		while (it.hasNext()) {
			Double d = (Double) it.next();
			//			System.out.println(""+ d + "     "+allocatedStartMap.get(d));
		}
	}
}

//	操作内存分配和释放的类
class OperatorPanel extends JPanel {
	private JTextField allocField = new JTextField(10);
	private JTextField freeField = new JTextField(10);
	private ControlPanel controlPanel;
	private DispPanel dispPanel;
	private JComboBox methodSelectionCombo;
	
	//	该类的构造函数，负责操作界面的布局
	public OperatorPanel(ControlPanel panel, DispPanel disp) {
		controlPanel = panel;
		dispPanel = disp;

		setLayout(new GridLayout(3, 1));
		
		//	选择分配算法的下拉列表框
		methodSelectionCombo = new JComboBox();
		methodSelectionCombo.setEditable(false);
		methodSelectionCombo.addItem("最先适配算法"); //加入组合框内容
		methodSelectionCombo.addItem("最佳适配算法");

		JPanel selectPanel = new JPanel();
		selectPanel.setLayout(new GridLayout(3, 1));
		selectPanel.add(new JLabel(""));
		selectPanel.add(new JLabel("分区分配算法的选择"));
		selectPanel.add(methodSelectionCombo);

		ActionListener comboListener = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int selectIndex = methodSelectionCombo.getSelectedIndex();
				System.out.println(selectIndex);

				controlPanel.setAllocMethod(selectIndex);
			}
		};
		methodSelectionCombo.addActionListener(comboListener);
		
		//	分配和释放内存 的按钮
		JButton allocButton = new JButton("Alloc");
		JButton freeButton = new JButton("Free");

		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new GridLayout(3, 1));

		JPanel p1 = new JPanel();
		p1.setLayout(new FlowLayout());
		p1.add(new JLabel("内存大小"));
		p1.add(allocField);
		p1.add(allocButton);

		JPanel p2 = new JPanel();
		p2.setLayout(new FlowLayout());
		p2.add(new JLabel("进程编号"));
		p2.add(freeField);
		p2.add(freeButton);

		//	分配内存按钮的监听类
		ActionListener allocButtonL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {

				String strInput = allocField.getText();

				allocNewMemory(strInput);

			}
		};
		allocButton.addActionListener(allocButtonL);

		//	释放内存按钮的监听类
		ActionListener freeButtonL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				String strInput = freeField.getText();
				freeMemory(strInput);
			}
		};
		freeButton.addActionListener(freeButtonL);

		buttonPanel.add(new JLabel(""));
		buttonPanel.add(p1);
		buttonPanel.add(p2);
		
		//	退出按钮
		JButton exitButton = new JButton("Exit");

		ActionListener ExitL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				System.exit(0);
			}
		};
		exitButton.addActionListener(ExitL);

		JPanel exitPanel = new JPanel();
		exitPanel.setLayout(new FlowLayout());
		exitPanel.add(exitButton);

		JPanel bottomPanel = new JPanel();
		bottomPanel.setLayout(new GridLayout(3, 1));
		bottomPanel.add(new JLabel(""));
		bottomPanel.add(exitPanel);
		bottomPanel.add(new JLabel("                 012574     姜宏慧"));

		add(selectPanel);
		add(buttonPanel);
		add(bottomPanel);

	}
	
	//	释放内存的处理函数
	private void freeMemory(String strInput) {
		try {
			int ProcessID = Integer.parseInt(strInput.trim());
			//	System.out.println(ProcessID);
			//	让controlPanel的freeMemory(ProcessID)来真正处理释放内存
			controlPanel.freeMemory(ProcessID);

		} catch (Exception e) {
			freeField.setText("");

			dispPanel.updateTextArea(
				"Error          input wrong!\n        Please input the ProcessID\n");

			System.out.println("fail");
			//	System.out.println("input wrong");
		}
	}
	//	分配内存的处理函数
	private void allocNewMemory(String strInput) {
		try {

			double size = Double.parseDouble(strInput.trim());
			//			System.out.println("intput right");
			if (size < 0.0000001 || size > 640) {
				dispPanel.updateTextArea("Error          input wrong!\n");
				return;
			}
			//	让controlPanel的allocNewMemory(size)来真正处理分配内存
			controlPanel.allocNewMemory(size);

		} catch (Exception e) {
			allocField.setText("");
			dispPanel.updateTextArea("Error          input wrong!\n");
			//			System.out.println("input wrong");
		}

	}
}

//	控制内存分配信息和空闲内存信息的控制类
class ControlPanel extends JPanel {
	private GraphicPanel graphicPanel;
	private DispPanel dispPanel;
	//	allocatedMap存放着内存的分配信息，key是分配了的内存的起始地址，value是key开始的那块内存的长度
	private SortedMap allocatedMap = new TreeMap(); 
	
	private SortedMap allocatedStartMap = new TreeMap();
	//	freeMap存放着空闲内存的信息，key是一块空闲内存的起始地址，value 是key开始的空闲内存的长度
	private SortedMap freeMap = new TreeMap();
	private ArrayList allocatedMarkArray = new ArrayList();
	private int MemoryID = 0;
	private boolean isBestAdapted = false;

	//	该类的构造函数
	public ControlPanel(GraphicPanel panel) {
		graphicPanel = panel;
		setLayout(new GridLayout(2, 1));
		dispPanel = new DispPanel();
		OperatorPanel operatorPanel = new OperatorPanel(this, dispPanel);

		add(dispPanel);
		add(operatorPanel);

		Double start = new Double(0);
		Double length = new Double(640);
		freeMap.put(start, length);

	}
	
	public ArrayList getAllocatedMarkArray() {
		return allocatedMarkArray;
	}
	
	//	真正处理释放内存的函数
	public void freeMemory(int ProcessID) {

		// 找到要释放的那块内存
		Iterator it = allocatedStartMap.keySet().iterator();
		while (it.hasNext()) {
			//			System.out.println(ProcessID);
			Double startLocation = (Double) it.next();
			if (ProcessID
				== ((Integer) allocatedStartMap.get(startLocation)).intValue()) {
				//	System.out.println("aa");
				//	释放内存，也即修改维护内存分配信息的Map
				updateMaps(startLocation, ProcessID);

				//	dispMap(allocatedMap);
				
				// 	更新了的内存分配情况，通知模拟绘制内存分配的类，要求更新画面
				graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);
				dispPanel.upDateDisp(allocatedMap, allocatedStartMap);
				graphicPanel.repaint();

				dispPanel.updateTextArea(
					"free  Successful\n        The space of Process "
						+ ProcessID
						+ " has been freed! \n");

				return;
			}

		}
		dispPanel.updateTextArea(
			"Error          input wrong!\n        Please input the ProcessID that is existed\n");

	}
	

	//	当释放一块内存时，处理是否要和相邻的空闲内存合并
	private void modifyFreeMap() {
		Iterator it = freeMap.keySet().iterator();

		SortedMap temp = new TreeMap();
		double preStartFreeLocation = -1;
		double preFreeLength = -1;
		if (it.hasNext()) {
			Double freeStart = (Double) it.next();
			Double freeLength = (Double) freeMap.get(freeStart);
			preStartFreeLocation = freeStart.doubleValue();
			preFreeLength = freeLength.doubleValue();
		}
		while (it.hasNext()) {
			Double freeStart = (Double) it.next();
			Double freeLength = (Double) freeMap.get(freeStart);

			double temp1 = preStartFreeLocation + preFreeLength;
			double temp2 = freeStart.doubleValue();
			if (Math.abs(temp1 - temp2) < 0.00001) {

				double newLength = preFreeLength + freeLength.doubleValue();

				preFreeLength = newLength;

				temp.put(
					new Double(preStartFreeLocation),
					new Double(preFreeLength));

			} else {
				temp.put(
					new Double(preStartFreeLocation),
					new Double(preFreeLength));
				//update preStartFreeLocation and preFreeLength
				preStartFreeLocation = freeStart.doubleValue();
				preFreeLength = freeLength.doubleValue();

			}

		}
		temp.put(new Double(preStartFreeLocation), new Double(preFreeLength));
		//dispMap(temp);
		freeMap = temp;
		System.out.println("After modify freeMap");
		dispMap(freeMap);

	}
	
	//	更新内存维护信息的Map
	private void updateMaps(Double startLocation, int ProcessID) {
		
		Double length = (Double) allocatedMap.get(startLocation);
		//	更新内存分配信息的Map
		allocatedMap.remove(startLocation);
		allocatedStartMap.remove(startLocation);
		//		dispMap(freeMap);
		//	更新空闲内存的分布信息
		freeMap.put(startLocation, length);
		//	当释放一块内存时，处理是否要和相邻的空闲内存合并
		modifyFreeMap();

	}

	//	真正处理分配内存的函数
	public void allocNewMemory(double size) {
		//	System.out.println("Receive command");
		//	dispMap(freeMap);
		//	看是否能够直接分配给空闲内存
		if (setAllocatedMap(size)) {
			//	System.out.println("true");
			graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);
			

			
			dispPanel.upDateDisp(allocatedMap, allocatedStartMap); 

			graphicPanel.repaint();
			return;
		}
		//		System.out.println("aaaaaaa");
		dispPanel.updateTextArea("No enough space to alloc\n");
		// 当没有直接分配的空闲内存时，对当前的内存分配进行紧缩
		adjustMemory();
		dispPanel.updateTextArea("After contraction\n");
		//	紧缩后，再次尝试分配
		if (setAllocatedMap(size)) {
			//	System.out.println("true");
			// 再次分配，成功后更新显示
			graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);

			for (int i = 0; i < allocatedMarkArray.size(); ++i) {
				//	System.out.println(allocatedMarkArray.get(i));
			}

			dispPanel.upDateDisp(allocatedMap, allocatedStartMap);  

			graphicPanel.repaint();
			return;
		}
		//	再次尝试分配，失败后，提供提示信息
		dispPanel.updateTextArea("Alloc failed   The Process is too large\n");

	}
	// 对空闲内存进行紧缩的函数
	private void adjustMemory() {
		//System.out.println("aaaaaaa");
		//		dispMap(allocatedMap);

		SortedMap tempMap = new TreeMap();

		Iterator it = allocatedMap.keySet().iterator();
		double currentLocation = 0;
		//		dispMap(allocatedMap);
		while (it.hasNext()) {
			//System.out.println("aaaaaaa");
			Double d = (Double) it.next();
			Double length = (Double) allocatedMap.get(d);
			Double currentDouble = new Double(currentLocation);
			//	allocatedMap.remove(d);
			tempMap.put(currentDouble, length);
			currentLocation = currentLocation + length.doubleValue();

			Integer ProcessID = (Integer) allocatedStartMap.get(d);
			allocatedStartMap.remove(d);
			allocatedStartMap.put(currentDouble, ProcessID);

		}
		//		dispMap(tempMap);
		allocatedMap = tempMap;
		//		dispMap(allocatedMap);
		//		System.out.println("aaaaa");
		freeMap.clear();
		Double freeStart = new Double(currentLocation);
		freeMap.put(freeStart, new Double(640 - freeStart.doubleValue()));

		//		dispMap(freeMap);

		graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);

		dispPanel.upDateDisp(allocatedMap, allocatedStartMap); //yao 
		//		dispMap(allocatedMap);
		graphicPanel.repaint();

		return;

	}
	
	//	两种内存分配方式的切换
	public void setAllocMethod(int selectIndex) {
		if (selectIndex == 0) {
			isBestAdapted = false;
		} else {
			isBestAdapted = true;
		}

	}
	
	//	使用最佳适应算法的内存分配函数
	private boolean canBeBestAdapted(double size) {
		Iterator itFreeMap = freeMap.keySet().iterator();
		double min = -1;
		Double minStart = null;
		Double StartRect = null;
		Double LengthOfRect = null;
		double freeLength = 0;
		//	看是否能够直接分配给空闲内存
		while (itFreeMap.hasNext()) {
			StartRect = (Double) itFreeMap.next();
			LengthOfRect = (Double) (freeMap.get(StartRect));

			freeLength = LengthOfRect.doubleValue();
			if (freeLength >= size) {
				//	System.out.println(freeLength);
				if (min == -1) {
					minStart = StartRect;
					min = freeLength;
					//	System.out.println(StartRect+ "   "+freeLength);
				} else {
					if (min > freeLength) {
						minStart = StartRect;
						min = freeLength;
						//	System.out.println(StartRect+ "   "+freeLength);
					}
				}
			}
		}
		if (min != -1) {

			Double Length = new Double(size);
			allocatedMap.put(minStart, Length);

			Integer numID = new Integer(++MemoryID);
			allocatedMarkArray.add(numID);
			allocatedStartMap.put(minStart, numID);
			//	dispMap(allocatedStartMap);

			Double newStart = new Double(minStart.doubleValue() + size);
			Double newLength = new Double(min - size);
			freeMap.remove(minStart);
			freeMap.put(newStart, newLength);
			//		dispMap(freeMap);
			dispPanel.updateTextArea(
				"Alloc Successful\n        Process "
					+ MemoryID
					+ " Start from "
					+ minStart
					+ "K  Length "
					+ Length
					+ "K !\n");

			return true;
		} else {
			return false;
		}
	}
	
	//	进行内存分配的调度
	private boolean setAllocatedMap(double size) {
		
		if (isBestAdapted) {
			//	调用最佳适应算法
			boolean result = canBeBestAdapted(size);
			return result;
		}
		
		//	调用最先适应算法
		Iterator itFreeMap = freeMap.keySet().iterator();
		while (itFreeMap.hasNext()) {
			Double StartRect = (Double) itFreeMap.next();
			Double LengthOfRect = (Double) (freeMap.get(StartRect));

			double freeLength = LengthOfRect.doubleValue();
			//	System.out.println(freeLength);
			if (freeLength >= size) {
				Double Length = new Double(size);
				allocatedMap.put(StartRect, Length);

				Integer numID = new Integer(++MemoryID);
				allocatedMarkArray.add(numID);
				allocatedStartMap.put(StartRect, numID);
				//	dispMap(allocatedStartMap);

				Double newStart = new Double(StartRect.doubleValue() + size);
				Double newLength = new Double(freeLength - size);
				freeMap.remove(StartRect);
				freeMap.put(newStart, newLength);

				dispPanel.updateTextArea(
					"Alloc Successful\n        Process "
						+ MemoryID
						+ " Start from "
						+ StartRect
						+ "K  Length "
						+ Length
						+ "K !\n");
				//dispMap(allocatedMap);
				//				System.out.println("aaaaaaaaaaaaaa");
				//dispMap(freeMap);
				//				System.out.println("bbbbbbbbbbbbbb");

				return true;
			}

		}
		return false;

	}
	
	//	显示Map的辅助类
	public void dispMap(Map map) {
		Iterator it = map.keySet().iterator();
		while (it.hasNext()) {
			Double d = (Double) it.next();

			System.out.println(d + "   " + map.get(d));
		}
		System.out.println("-----------------");
	}

}

//	主框架的主Panel类
class MemoryAllocPanel extends JPanel {
	public MemoryAllocPanel() {

		setLayout(new GridLayout(1, 2));
		GraphicPanel graphicPanel = new GraphicPanel();
		ControlPanel controlPanel = new ControlPanel(graphicPanel);

		add(graphicPanel);
		add(controlPanel);
	}

}
