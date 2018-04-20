#!/bin/sh
#first.cgi
#display a page with button
#echo "Content-type:text/html charset=GB2312"
echo ""
echo "<HTML>"
echo "<head>"
echo "    <meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"
echo "    <title>Welcome to WLTC Test</title>"
echo "    <meta name='author' content='Jack'>"
echo "    <!-- Date: 2007-08-30 -->"
echo "</head>"
echo "<body>"
echo "    <meta http-equiv='Content-Type' content='image/jpeg; charset=utf-8'>"
echo "    <div id='Layer1' style='position:absolute; width:100%; height:100%; z-index:-1'>"
echo "    <img src='http://169.254.108.250/pic/srdc.jpg' height='100%' width='100%'/>"
echo "    </div>"
echo "    <meta http-equiv='Content-Type' content='text/html; charset=GB2312'>"
echo "    <H1><CENTER> WELCOME TO WLTC TEST</CENTER></H1>"
echo "    <HR>"
echo "    <BR>"
#echo "<H2><CENTER> WLTC Test</CENTER></H2>"
#echo "    <form action='http://169.254.108.250/cgi-bin/trans.cgi' method='post'>"
#echo "    <CENTER><button id='transbtn' name='subject' type='submit' value='click' disabled=true><H2><CENTER>同步数据</CENTER></H2></button></CENTER>"
#echo "    </form>"
#echo "    <form action='http://169.254.108.250/cgi-bin/begin_test.cgi' method='post'>"
#echo "    <CENTER><button id='startbtn' name='subject' type='submit' value='click' disabled=true><H2><CENTER>开始测试</CENTER></H2></button></CENTER>"
#echo "    </form>"
echo "    <form action='http://169.254.108.250/cgi-bin/upload.cgi' method='post' enctype='multipart/form-data'>"
echo "    <CENTER><input type='file' name='file' style='font-size:20px' value='' />"
echo "    <button type='submit' name='submit' style='font-size:20px' value='OK'>OK</CENTER>"
echo "    </form>"
#echo "<script>
#echo "    function enableElement(ch)
#echo "    {
#echo "    document.getElementById(ch).disabled=false;
#echo "    }
#echo "</script>
echo "</body>"
echo "</HTML>"
