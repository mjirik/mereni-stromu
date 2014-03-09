<?php
// $Id%

echo "<div class=\"vnitrni\">";
echo $text;
echo "<table>";
echo "<tr>";
echo "<td></td>";
echo "<td>".odkaz("Ok",$hrefOk)."</td>";
echo "<td>".odkaz("Storno",$hrefStorno)."</td>";
echo "<td></td>";
echo "</tr>";
echo "</table>";
echo "</div>";

//include ('template.class.php');
//include ('funkce.php');
//$odkaz = connect_db();

//$all = & new Template('./tmpl/all.tmpl.php');
//$body = & new Template('./tmpl/ok.tmpl.php');
?>
