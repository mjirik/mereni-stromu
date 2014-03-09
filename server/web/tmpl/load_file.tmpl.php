<?php
// $Id: load_file.tmpl.php 35 2007-04-24 18:58:46Z Mira $
echo "Data jsou načtena ze souborů <b>mereni.dat</b> a <b>zakaznici.dat</b> v 
adresáři <b>db</b>.<br> Sloupce jsou odděleny středníkem. <br>" ;
echo "<h3>zakaznici.dat</h3>";
echo "IDZakaznik;Jmeno;Datum<br>";
echo "<h3>mereni.dat</h3>";
echo "IDZakaznik;IDDruh_dreva;Delka;Prumer;IDManipulace,IDDalsi_zpracovani <br>";
echo "
<a href=load_file.php?start=true> Načti </a>";
// echo "<form method=\"post\" action=\"load_file.php\">";
// echo "<table>";
// formln("Jméno souboru:","<input name=\"jmeno\" value=\"".$zaznam["Jmeno"]."\">"); 
// formln("","<input type=\"Submit\" name=\"odesli\" size = 10 value = \"Uložit\">");
// echo "</table>\n";
// echo "</form>\n";
?>
