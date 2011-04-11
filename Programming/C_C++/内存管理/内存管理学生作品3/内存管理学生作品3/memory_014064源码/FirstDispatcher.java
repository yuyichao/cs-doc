import java.util.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * 
 * @author Liu Junhui
 *
 * 首次适应算法的分配类，继承分配基类
 * 分配算法,找到第一个大于或等于请求大小的分区就分配
 */
class FirstDispatcher extends MyDispatcher {
	public FirstDispatcher() {
		freeList = new ArrayList();
		SpaceNode initNode = new SpaceNode(0, Utility.initMemory,0);
		freeList.add(initNode);
		useList = new ArrayList();
	}
	
	//合并分区的函数
	public void addFreeNode(SpaceNode freeNode)
	{		
		SpaceNode preNode = null;
		SpaceNode nextNode = null;
		SpaceNode node = null;
		int preAddr = -1;
		int nextAddr = -1;
		
		for (int i=0;i<freeList.size();i++)
		{
			node = (SpaceNode)freeList.get(i);
			if (node.getAddrFrom() < freeNode.getAddrFrom())
			{
				preAddr = i;
			}
			else if (node.getAddrFrom() > freeNode.getAddrFrom())
			{
				nextAddr = i;
				break;
			}
		}
		
		//空闲链表为空,直接插进去
		if (freeList.size() == 0)
		{
			freeNode.setDispatchID(0);
			freeList.add(freeNode);
			return ;
		}
		
		if (preAddr != -1)
		{
			preNode = (SpaceNode)freeList.get(preAddr);
			
			//与前面相邻分区合并
			if (preNode.getAddrNext() == freeNode.getAddrFrom())
			{
				preNode.setLen(preNode.getLen() + freeNode.getLen());
				if (nextAddr != -1)
				{
					nextNode = (SpaceNode)freeList.get(nextAddr);
					//还与后面相邻分区合并
					if (freeNode.getAddrNext() == nextNode.getAddrFrom())
					{
						preNode.setLen(preNode.getLen() + nextNode.getLen());
						freeList.remove(nextAddr);
					}
				}
				return;
			}
		}
		if (nextAddr != -1)
		{
			nextNode = (SpaceNode)freeList.get(nextAddr);
			//仅与后面相邻分区合并
			if (freeNode.getAddrNext() == nextNode.getAddrFrom())
			{
				nextNode.setAddrFrom(freeNode.getAddrFrom());
				nextNode.setLen(nextNode.getLen() + freeNode.getLen());
				return;
			}		
		}
		
		//无法合并,插到适当位置
		freeNode.setDispatchID(0);
		freeList.add(preAddr+1,freeNode);		
	}
}


