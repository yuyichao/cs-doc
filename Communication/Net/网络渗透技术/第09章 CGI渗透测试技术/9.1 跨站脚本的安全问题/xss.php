<?php
/* xss.php
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在跨站脚本漏洞的演示程序
*/

setcookie ("TestCookie", "xss");

$input = stripslashes($input);
echo "test string: ".$input;
?>
