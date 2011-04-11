import java.util.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * 动态分区算法的分配基类
 */
public class MyDispatcher {
	protected ArrayList freeList; //代表空闲分区的链表
	protected ArrayList useList; //代表使用分区的链表
	private boolean hasTight = false; //标志已经紧缩

	/* 分配函数：
	 * 输入需要的字节数，返回代表空闲分区的内存地址,如果返回-1表示内存空间不足*/
	public int alloc(int nbyte) {
		System.out.println("### alloc memory " + nbyte + " ###");
		SpaceNode node;
		int len;

		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			len = node.getLen();

			if (len >= nbyte) {
				if (len == nbyte) {
					//把该分区分配给请求进程

					int dispatchID = Utility.getDispatchID();
					node.setDispatchID(dispatchID);
					addUseNode(node);
					//把该分区从空闲链表中删去
					freeList.remove(i);
					System.out.println(toString());
					return dispatchID;
				}

				//需要分割分区
				int dispatchID = Utility.getDispatchID();
				SpaceNode useNode =
					new SpaceNode(node.getAddrFrom(), nbyte, dispatchID);
				addUseNode(useNode);
				node.setAddrFrom(node.getAddrFrom() + nbyte);
				node.setLen(len - nbyte);
				System.out.println(toString());
				return dispatchID;
			}
		}

		//还没完成紧缩的时机！！！！！！
		if (hasTight == false) {
			tightNode();
			hasTight = true;
			return alloc(nbyte);
		}
		System.out.println("not enough memory!");
		return -1;
	}

	//使用链表是按地址升序连接的
	public void addUseNode(SpaceNode useNode) {
		SpaceNode node;
		for (int i = 0; i < useList.size(); i++) {
			node = (SpaceNode) useList.get(i);
			if (useNode.getAddrFrom() < node.getAddrFrom()) {
				useList.add(i, useNode);
				return;
			}
		}
		useList.add(useNode);
	}

	//释放算法,如果要释放后可以与相邻分区合并就合并
	public boolean free(int dispatchID) {
		System.out.println("### free dispatchID " + dispatchID + " ###");
		for (int i = 0; i < useList.size(); i++) {
			SpaceNode node = (SpaceNode) useList.get(i);
			if (dispatchID == node.getDispatchID()) {
				addFreeNode(node); //合并分区函数
				useList.remove(i);
				System.out.println(toString());
				hasTight = false;
				return true;
			}
		}
		return false;
	}

	//合并相邻分区的函数
	//注意：两种适应算法的差别之处，子类需继承
	public void addFreeNode(SpaceNode freeNode) {
	}

	public void tightNode() {
		System.out.println("tight!");
		if (useList.size() == 0) {
			return;
		}

		SpaceNode node;
		SpaceNode lastNode = (SpaceNode) useList.get(0);
		lastNode.setAddrFrom(0);
		int useSpace = lastNode.getLen();
		for (int i = 1; i < useList.size(); i++) {
			node = (SpaceNode) useList.get(i);
			useSpace += node.getLen();
			node.setAddrFrom(lastNode.getAddrNext());
			lastNode = node;
		}

		SpaceNode freeNode =
			new SpaceNode(
				lastNode.getAddrNext(),
				Utility.initMemory - useSpace,
				0);

		int useLen = freeList.size();
		for (int i = 0; i < useLen; i++) {
			freeList.remove(0);
		}
		freeList.add(freeNode);
		System.out.println(toString());
	}

	public ArrayList getFreeList()
	{
		return freeList;
	}
	
	public ArrayList getUseList()
	{
		return useList;
	}

	public String toString() {
		String str;
		str = "------------------Free List-----------------\n";
		for (int i = 0; i < freeList.size(); i++) {
			SpaceNode node = (SpaceNode) freeList.get(i);
			str = str + node.toString() + "\n";
		}
		str += "-----------------Use List-----------------\n";
		for (int i = 0; i < useList.size(); i++) {
			SpaceNode node = (SpaceNode) useList.get(i);
			str = str + node.toString() + "\n";
		}
		return str;
	}
}
