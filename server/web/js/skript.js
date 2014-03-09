/**
 * $Id: skript.js 77 2007-08-13 16:43:56Z Mira $
 * @file skript.js
 * Soubor obsahuje javaskriptové funkce webové aplikace.  
 */
 
/**
 * Funkce přepočítá cenu při změně na forumláři pro editaci zakázky. 
 */
function prepoctiCenu(select,chbx_dvakrat, chbx_manipulace, slct_oper, span_cena){
  select = getE(select);
  chbx_dvakrat = getE(chbx_dvakrat);
  chbx_manipulace = getE(chbx_manipulace);
  slct_oper = getE(slct_oper);
  
  var cena;
  
  if (select[select.selectedIndex].value < 4){
    cena=parseFloat(getE('Cena_1').value);
  }
  else{
    cena=parseFloat(getE('Cena_4').value);
  }
  
  if(chbx_dvakrat.checked ){
    cena += parseFloat(getE('Cena_dvakrat').value);
  }
  if(chbx_manipulace.checked ){
    cena += parseFloat(getE('Cena_man').value);
  }
  if (slct_oper[slct_oper.selectedIndex].value == 2){
    cena +=parseFloat(getE('Cena_dalsi_1').value);
  }
  else if (slct_oper[slct_oper.selectedIndex].value == 3){
    cena +=parseFloat(getE('Cena_dalsi_2').value);
  }
  
  span_cena=getE(span_cena);
  span_cena.innerHTML=cena;
}
