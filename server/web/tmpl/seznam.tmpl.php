<script type="text/javascript" language="javascript">
function printPage(){
  
    // Do print the page
  if (typeof(window.print) != 'undefined') {
    window.print();
  }
}
</script>
<div class="print" style="width:650px;">
<div class="print_ignore">
<table><tr><td>
<input type="button" id="print" value="Vytisknout" onclick="printPage()" ><td></tr>
<tr><td>
<?php
echo odkaz("Zpět","edit_seznam.php?id=".$_GET["id"]); 
?>
</td></tr>
</table>
<hr size=3 align="center">
</div>
<div class="print">
<div class="print_head">
<?php
  $radek = 0;
  $MAX_RADEK = 38;

  $radek = 3; // mezery kolem adresy
  $pocet = 0;
  echo "<h3>".$zakaznik["Jmeno"]."</h3>";
  $radek++;
  
  $patterns[]="/\n/";
  $replacements[]="<br>\n";
  $adr = preg_replace($patterns,$replacements,$zakaznik["Adresa"],-1,$pocet);
  echo $adr;
  
  $radek += $pocet; 
?>
</div>
</div>
<hr>
<div class="print">
  
  <table cellspacing=2 border=0 width="100%">
  <?php
  $radek++; // linka nad tabulkou
  
  $zahlavi = "<th>Délka [m]</th>
              <th>Průměr [cm]</th>
              <th>Objem [m³]</th>";
  $mezisoucet = NULL;
  if (!empty($zaznamy)){
  foreach($zaznamy as $key => $zaznam){
    $mezisoucet = $mezisoucty[$zaznam["IDDruh_dreva"]>3][$zaznam["IDManipulace"]]
    [$zaznam["IDDvakrat"]][$zaznam["IDDalsi_zpracovani"]];
    
    // výpi popisku  ($zaznamy[$key-1] != NULL)&&
    if ($radek > $MAX_RADEK){ ?>
      <tr class="print_ignore"><td align="center" colspan=3>
      - - - - - - - - - - - - - - - STRÁNKOVÝ ZLOM - - - - - - - - - - - - - - - </td></tr>
      <tr style="page-break-before: always;"><td colspan=3>
      <?php echo $mezisoucet["Popisek"]; ?>
      </td></tr>
      <tr>
      <?php echo $zahlavi; $radek = 2; // hlavička?>
      </tr>
    <?php }
    else if ( (
      (($zaznamy[$key-1]["IDDruh_dreva"] > 3)!= ($zaznam["IDDruh_dreva"] > 3))||
      ($zaznamy[$key-1]["IDManipulace"] != $zaznam["IDManipulace"])||
      ($zaznamy[$key-1]["IDDvakrat"] != $zaznam["IDDvakrat"])||
      ($zaznamy[$key-1]["IDDalsi_zpracovani"] != $zaznam["IDDalsi_zpracovani"])
    )){ ?>
      <?php //odřádkování ?>
      <?php if ($zaznamy[$key - 1] != null){ ?>
        <?php $radek ++; ?>
        <tr><td>&nbsp;</td></tr>
        <tr><td colspan=3>
      <?php } ?>
      <?php echo $mezisoucet["Popisek"]; ?>
      </td></tr>
      <tr>
      <?php echo $zahlavi; $radek += 2;?>
      </tr>
    
    <?php } ?>
  
    <?php 
    // Výpis dat
    //echo "<tr style=\"page-break-before: always;\">";
    echo "<tr style=\"page-break-before: auto;\">";
    echo "<td align=\"center\">".$zaznam["Delka"]."</td>".
    "<td align=\"center\">".$zaznam["Prumer"]."</td>";
    //"<td align=\"center\">".$zaznam["Druh_dreva"]."</td>";
    echo "<td align=\"center\">".$zaznam["Objem"]."</td>";
    //echo "<td>".$zaznam["Cena"]."</td>";
    echo "</tr>\n";
    
    //Mezisoučty ( ) && 
    if (
      (($zaznamy[$key+1]["IDDruh_dreva"] > 3)!= ($zaznam["IDDruh_dreva"] > 3))||
      ($zaznamy[$key+1]["IDManipulace"] != $zaznam["IDManipulace"])||
      ($zaznamy[$key+1]["IDDvakrat"] != $zaznam["IDDvakrat"])||
      ($zaznamy[$key+1]["IDDalsi_zpracovani"] != $zaznam["IDDalsi_zpracovani"])
    ){?>
        <tr>
        <td colspan=3 align="center">
        <i>
        Mezisoučet
        <?php echo sprintf ("%1.2f", $mezisoucet["Cena"]); $radek++; ?> kč
        </i>
        </td>
        </tr>
        
  
    
        
    <?php
    }
    $radek++;
  }
  }
  ?>
  </table>
  <hr>
  <table cellspacing=2 border=0 width="50%">
  <tr>
  <td align="center"></td>
  <td align="center"><b>Cena celkem</b></td>
  <td align="center"><?php echo sprintf ("%01.2f", $cenaCelkem) ?> kč</td>
  </tr>
  </table>
</div>
</div>
