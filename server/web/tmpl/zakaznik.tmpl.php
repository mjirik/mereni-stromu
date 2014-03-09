<?php
if (!empty($echo)){
echo $echo;
}
else {
	

  echo "<table>";
//     while ($zaznam=MySQL_Fetch_Array($vysledek)):
//     foreach($zakaznik as $zaznam){
    $adr = ereg_replace("\n","<br>\n",$zaznam["Adresa"]);
    echo "<tr><td>Jméno</td><td class=\"jmeno\">".$zaznam["Jmeno"]."<td></tr>\n";
    formln("IČO:",$zaznam["ICO"]);
    formln("DIČ:",$zaznam["DIC"]);
    formln("Adresa:",$adr);
    formln("Telefon:",$zaznam["Telefon"]);
    formln("E-mail:",$zaznam["Email"]);
    formln("","<a href=\"edit_zakaznik.php?id=".$_GET["id"]."\">Edit</a>");
    formln("","<a href=\"seznamy.php?id=".$_GET["id"]."\">Zobrazit seznamy</a>");
    //input type=\"Submit\" name=\"odesli\" size = 10 value = \"Uložit\">");
//     }
  echo "</table>";
}  
?>
