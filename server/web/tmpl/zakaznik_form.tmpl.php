<?php
formln("Jméno:","<input name=\"jmeno\" value=\"".$zakaznik["jmeno"]."\">"); 
formln("IČO:","<input name=\"ico\" value=".$zakaznik["ico"].">");
formln("DIČ:","<input name=\"dic\" value=".$zakaznik["dic"].">");
formln("Adresa:","<textarea name=\"adresa\" warp=off style=\"width:240px;height:100px\">".$zakaznik["adresa"]."</textarea>");
formln("Telefon:","<input name=\"telefon\" value=".$zakaznik["telefon"].">");
formln("E-mail:","<input name=\"email\" value=".$zakaznik["email"].">");
?>
