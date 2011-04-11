<%
SQL = ""
If Request("str")<>"" Then
	SQL="SELECT * FROM article WHERE data like '%" &_
		Request("str") & "%'"
elseif Request("id")<>"" Then
	SQL="SELECT * FROM article WHERE id=" &_
		Request("id")
elseif Request("link")<>"" Then
	SQL="SELECT * FROM article WHERE link='" &_
		Request("link") & "'"
end if

if(SQL <> "") then
	StrSQL="driver={SQL Server};server=(local);"&_
			"database=lalala;uid=xfocus;pwd=super;"
	set conn=server.createobject("ADODB.CONNECTION")
	conn.open StrSQL
	Set rs = conn.Execute(SQL)
	If NOT rs.BOF Then
		while(NOT rs.EOF)
			Response.Write ""&_
				"ID  : " & rs("ID") & VBCRLF & "<br>" &_
				"Data: " & rs("Data")& VBCRLF & "<br>" &_
				"Link: " & rs("link") & VBCRLF & "<br><br>"
			rs.MoveNext
		wend
	Else
		Response.Write "你要的数据不存在!"
	End If
	Set rs=Nothing
	conn.close
End if
%>
<Form action="search.asp">
文章ID      ：<Input Name="id"><P>
文章特征字符：<Input Name="str"><P>
文章来源    ：<Input Name="link"><P>
<P>
<Input type="submit" Value="确定">
</Form>
