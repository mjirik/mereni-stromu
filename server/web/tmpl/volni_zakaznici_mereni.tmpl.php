<?php if (!empty ($zaznamy)){ ?>
  <div class="obtekany">
  <div class="vnitrni">
  <table class="longtab">
  <tr>
  <th class="odsazeni"></th><th>Identifikátor</th><th>Počet<br>stromů</th>
  <th></th><th></th></tr>
  <?php $datum = null;
  
      foreach($zaznamy as $zaznam){
        if (strcmp($datum,$zaznam["Datum"]) != 0){
          echo "<tr><td COLSPAN=3><h4>".sql_datum($zaznam["Datum"])."</h4></td></tr>";
        }
        $datum = $zaznam["Datum"];
        echo "<tr";
        //zvýraznění vybraného
        if ($zaznam["IDZakaznik_mereni"] == $_GET["id"]){
          echo " class=\"focus\"";
        }
        echo ">";
        echo "<td></td><td>".$zaznam["Jmeno"]."</td><td align=\"center\">".
        $zaznam["Pocet_stromu"].
        "</td><td><a href=\"nova_mereni.php?id="
        .$zaznam["IDZakaznik_mereni"]."\">Vyber"."</a></td>";
        echo "<td><a href=\"nova_mereni.php?novy=1&amp;id=".
        $zaznam["IDZakaznik_mereni"].
        "\">Nový zákazník</a></td>";
        if ($zaznam["nej_zakaznici"]!=null){
          foreach($zaznam["nej_zakaznici"] as $nej_zakaznik){
            echo "<td>".odkaz($nej_zakaznik["Jmeno"],
              "nova_mereni.php?id="
              .$zaznam["IDZakaznik_mereni"]."&amp;IDZakaznik=".
              $nej_zakaznik["IDZakaznik"])."</td>";
          
          }
        }
        echo "</tr>\n";
      }
      ?>
      </table>
      </div>
      </div>
  
  <?php if (!empty($zakaznici)){ ?>
    <div class="druheOkno">
    <div class="vnitrni">
    <h2><?php echo "$jmeno"; ?></h2>
    <table>
    <tr><th>Jméno</th></tr>
    <?php
    foreach ($zakaznici as $zaznam){
      echo "<tr";
        //zvýraznění vybraného
        if ($zaznam["IDZakaznik"] == $_GET["IDZakaznik"]){
          echo " class=\"focus\"";
          // zjištění jména zákazníka pro další práci
          $jmeno_zakaznika = $zaznam["Jmeno"];
        }
      echo ">";
      echo "<td>".$zaznam["Jmeno"]."</td>";
      echo "<td>";
      echo odkaz("Zvolit","nova_mereni.php?id=".
      $_GET["id"]."&amp;IDZakaznik=".$zaznam["IDZakaznik"]);
      /*echo ref_ok("Zvolit","nova_mereni.php?id=".
      $_GET["id"]."&amp;IDZakaznik=".$zaznam["IDZakaznik"],
      "nova_mereni.php",
      "Opravdu nahradit zákazníka <b>".$jmeno."</b> 
      zákazníkem <b>".$zaznam["Jmeno"]."</b>");*/
      echo "</td>";
      echo "</tr>";
    }
    ?>
    </table>
    </div>
    </div>
    <?php } ?>
  <?php if (!empty($_GET["IDZakaznik"])) {   ?>
    <div class="druheOkno">
    <div class="vnitrni">
    <h2><?php echo $jmeno_zakaznika;?></h2>
    
    <table>
    <tr><th>Zakázka</th></tr>
    <tr><td>
    <?php
      $ok_msg = "Opravdu přiřadit měření od identifikátoru <b>$jmeno</b> <br> do 
      nové zakázky k zákazníkovi <b>$jmeno_zakaznika</b>?" ;
      
      echo ref_ok("Nová zakázka",
      "nova_mereni.php?id=".$_GET["id"]."&amp;IDZakaznik=".
      $_GET["IDZakaznik"]."&amp;IDSeznam=-1",
      "nova_mereni.php?id=".$_GET["id"]."&amp;IDZakaznik=".
      $_GET["IDZakaznik"],$ok_msg
      );?>
    </td></tr>
    
    <?php 
    if (!empty($zakazky)) {
      foreach($zakazky as $zakazka){
      $zakazka["Nazev"]=$zakazka["IDSeznam"]." : ".sql_datum($zakazka["Datum"]);
        $ok_msg = "Opravdu přiřadit měření od identifikátoru <b>$jmeno</b> <br>
        do zakázky <b>".$zakazka["Nazev"]."</b> k zákazníkovi <b>
        $jmeno_zakaznika</b>?" ;
        
        echo "<tr style=\"text-align:right;\"><td>".$zakazka["Nazev"]."</td>";
        echo "<td>".
        ref_ok("Přiřadit",
          "nova_mereni.php?id=".$_GET["id"]."&amp;IDZakaznik=".
          $zaznam["IDZakaznik"]."&amp;IDSeznam=".$zakazka["IDSeznam"],
          "nova_mereni.php?id=".$_GET["id"]."&amp;IDZakaznik=".
          $_GET["IDZakaznik"], $ok_msg)."</td></tr>";
      } 
      }
      ?>
    </table>
    </div>
    </div>

  <?php } ?>
<?php
  
  if (!empty($add)){
    echo "<div class=\"druheOkno\">";
    echo "<div class=\"vnitrni\">";
    echo $add;
    echo "</div>";
    echo "</div>";
  }

}
else {
  echo "<div class=\"druheOkno\">";
  echo "<div class=\"vnitrni\">";
  echo "<div class=\"chyba_vnitrni\">";
  echo "Nenalezen žádný záznam.";
  echo "</div>";
  echo "</div>";
  echo "</div>";
}

  //echo "</div>";
?>
