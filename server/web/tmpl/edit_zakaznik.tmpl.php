<?php if ($samostatne == true) {?>
<div class="vnitrni">
<?php } ?>
<?php if (!empty($echo)){ ?>
  <div class="chyba_vnitrni">
  <?php echo_echo($echo); ?>
  </div>
<?php }
  //echo "<h2>$jmeno<h2>";
  echo "<form method=\"post\" action=\"$action\">";
  echo "<table>";
  input_form("Jméno:","jmeno",$zakaznik["Jmeno"]); 
  input_form("IČO:","ico", $zakaznik["ICO"]);
  input_form("DIČ:","dic", $zakaznik["DIC"]);
  formln("<label for=\"adr\">Adresa:</label>","<textarea id=\"adr\" name=\"adresa\" warp=off style=\"width:240px;height:100px\">".$zakaznik["Adresa"]."</textarea>");
  input_form("Telefon:","telefon", $zakaznik["Telefon"]);
  input_form("E-mail:","email",$zakaznik["Email"]);
  formln("","<input class=\"tlacitko\" type=\"Submit\" name=\"odesli\" size = 10 value = \"Uložit\">");
  echo "</table>\n";
  echo "</form>\n";
?>

<?php if ($samostatne == true){?>
</div>
<?php } ?>
