import java.util.*;

/*
 * Created on 2004-5-25
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * 最佳适应算法的分配类，继承分配基类
 */
public class BestDispatcher extends MyDispatcher {

	public BestDispatcher() {
		freeList = new ArrayList();
		SpaceNode initNode = new SpaceNode(0, Utility.initMemory, 0);
		freeList.add(initNode);
		useList = new ArrayList();
	}

	//注意：该算法的freeList是一个按空闲分区大小升序链接的链
	//      寻找相邻的分区要遍历freeList，物理上的相邻不代表freeList上相邻
	public void addFreeNode(SpaceNode freeNode) {
		SpaceNode preNode = null;
		SpaceNode nextNode = null;
		SpaceNode node = null;
		SpaceNode insertNode = null; //合并后要插入到freeList的节点
		int preAddr = -1;
		int nextAddr = -1;

		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			//遍历寻找物理上相邻的前驱
			if (node.getAddrNext() == freeNode.getAddrFrom()) {
				preAddr = i;
			}
			//遍历寻找物力上相邻的后继
			if (freeNode.getAddrNext() == node.getAddrFrom()) {
				nextAddr = i;
			}
		}

		//合并相邻分区
		boolean hasRemove = false;
		if (preAddr != -1) {
			preNode = (SpaceNode) freeList.get(preAddr);
			preNode.setLen(preNode.getLen() + freeNode.getLen());
			insertNode = preNode;
			if (nextAddr != -1) {
				nextNode = (SpaceNode) freeList.get(nextAddr);
				insertNode.setLen(insertNode.getLen() + nextNode.getLen());
				if (nextAddr < preAddr)
				{
					freeList.remove(preAddr);
					hasRemove = true;
				}
				freeList.remove(nextAddr);
			}
			if (hasRemove == false) {
				freeList.remove(preAddr);
			}
		} else if (nextAddr != -1) {
			nextNode = (SpaceNode) freeList.get(nextAddr);
			nextNode.setAddrFrom(freeNode.getAddrFrom());
			nextNode.setLen(nextNode.getLen() + freeNode.getLen());
			insertNode = nextNode;
			freeList.remove(nextAddr);
		} else {
			insertNode = freeNode;
		}

		//插入到空闲链表
		int insertLen = insertNode.getLen();
		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			if (insertNode.getLen() <= node.getLen()) {
				insertNode.setDispatchID(0);
				freeList.add(i, insertNode);
				return;
			}
		}
		insertNode.setDispatchID(0);
		freeList.add(insertNode);
	}
}
