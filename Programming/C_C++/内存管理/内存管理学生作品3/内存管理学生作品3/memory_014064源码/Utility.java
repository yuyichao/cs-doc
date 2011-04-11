/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class Utility {

	//用于分配DispatchID
	static int dispatchID = 0;
	static int getDispatchID() {
		return ++dispatchID;
	}
	static void resetDispatchID()
	{
		dispatchID = 0;
	}

	//初始参数
	public static final int initMemory = 640;
	public static final MyDispatcher initDispatcher = new FirstDispatcher();
	
	//界面参数
	public static final int WIDTH = 510;
	public static final int HEIGHT = 600;
	
	//显示参数
	public static final double KILO = 0.65;
	public static final double MEMORYWIDTH = 220;
	public static final double SPACE = 20;
}
