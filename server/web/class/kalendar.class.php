<?php
/**
 * $Id: kalendar.class.php 77 2007-08-13 16:43:56Z Mira $
 */
class kalendar {
  /**
   * Funkce vrací počet dnů v měsíci.
   */  
  function pocetDnu($mesic, $rok){ 
    //return cal_days_in_month(CAL_GREGORIAN, $mesic, $rok);
    return date('j', mktime(0,0, 0, $mesic+1, 0, $rok));
  }
  
  /**
   * Funkce vrací číslo prvního dne v týdnu.
   */
  function prvniDen($mesic, $rok){
    $anglickeporadi = date("w", mktime(0, 0, 0, $mesic, 1, $rok));
    return ($anglickeporadi==0) ? 7 : $anglickeporadi;
  }

  
  function bunka ($radek, $sloupec, $PrvniDen, $PocetDnu){ 
    $dny=Array(1=>"Po", "Út", "St", "Čt" ,"Pá", "So", "Ne");
    if ($sloupec==1) return $dny[$radek];
      $chcivratit = ($sloupec-2)*7 + $radek - $PrvniDen+1;
    if ($chcivratit < 1 || $chcivratit > $PocetDnu) 
      return "&nbsp;"; 
    else 
      return $chcivratit;
  }
  
  function vypis ($mesic, $rok){
    $mesice=Array(1=>"leden", "únor", "březen" ,"duben", "květen" ,"červen" ,
    "červenec" , "srpen", "zárí" , "říjen" , "listopad", "prosinec");
    //kontroly
    if (!is_numeric($mesic)) return "Mesíc musí být císlo!" ;
    if (!is_numeric($rok)) return "Rok musí být císlo!" ;
    if ($mesic<1 || $mesic>12) return "Mesíc musí být císlo od 1 do 12" ;
    if ($rok<1980 || $rok>2050) return "Rok musí být císlo od 1980 do 2050" ;
    // zjištení poctu sloupcu
    $PocetDnu = $this->pocetDnu($mesic, $rok); 
    $PrvniDen = $this->prvniDen($mesic,$rok);
    $sloupcu = date("W", mktime(0, 0, 0, $mesic, $PocetDnu-7, $rok)) - 
    date("W", mktime(0, 0, 0, $mesic, 1+7, $rok))+4;
    // vlastní kód
    echo "<TABLE border=\"1\" style=\"border-collapse: collapse\"width=\"",$sloupcu*30,"\">";
    echo "<TR><TD colspan=$sloupcu width=\"",$sloupcu*30,"\"align=\"center\">".$mesice[$mesic]."&nbsp;".$rok."</TD></TR>\n";
    for ($radek=1;$radek<=$sloupcu;$radek++){
      echo "<TR align=\"center\">";
      for ($sloupec=1; $sloupec<=7; $sloupec++){ 
        echo "<TD width=\"30\">".$this->bunka($sloupec, $radek, $PrvniDen, $PocetDnu)."</TD>";
      }
      echo "</TR>\n";
    }
    echo "</TABLE>";
  }
}
?>
