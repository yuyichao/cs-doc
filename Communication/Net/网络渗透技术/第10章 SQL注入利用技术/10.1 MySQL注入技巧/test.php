<?php
/* test.php
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  MySQL注入演示程序
*/

$link = mysql_connect("localhost", "root") or die("Could not connect: " . mysql_error() . "\n");
mysql_select_db("test") or die("Could not select database.\n");

$query = "SELECT * FROM member WHERE id={$_REQUEST['id']}";
$result = mysql_query($query) or die("Query error: " . mysql_error() . "\nQuery string: {$query}\n");

while($row = mysql_fetch_assoc($result)) {
    echo $row['id']." ".$row['name']."\n";
}

mysql_free_result($result);
mysql_close($link);
?>
