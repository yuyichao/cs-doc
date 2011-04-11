/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
 
/**
 * @author Liu Junhui
 *
 *代表空闲或使用链表的分区节点
 */
public class SpaceNode {
	private int addrFrom; //分区的地址起点
	private int len; //该分区的长度
	private int dispatchID; //标示已分配的分区ID，0表示为空闲分区

	public SpaceNode(int addrFrom, int len, int dispatchID) {
		this.addrFrom = addrFrom;
		this.len = len;
		this.dispatchID = dispatchID;
	}
/*
	public void reset(int addrFrom, int len) {
		this.addrFrom = addrFrom;
		this.len = len;
		this.dispatchID = dispatchID;
	}*/
	
	public int getLen()
	{
		return len;
	}
	
	public void setLen(int len)
	{
		this.len = len;
	}
	
	public int getAddrFrom()
	{
		return addrFrom;
	}
	
	public int getAddrNext()
	{
		return addrFrom + len;
	}
	
	public void setAddrFrom(int addrFrom)
	{
		this.addrFrom = addrFrom;
	}
	
	public String toString()
	{
		return "[DispatchID " + dispatchID + "] Address : " + addrFrom + "   Len : " + len;
	}
	
	
	public int getDispatchID()
	{
		return dispatchID;
	}
	
	public void setDispatchID(int dispatchID)
	{
		this.dispatchID = dispatchID;
	}
}

