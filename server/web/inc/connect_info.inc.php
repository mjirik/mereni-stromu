<?php
if (($_SERVER["SERVER_ADDR"]=="localhost")||($_SERVER["SERVER_ADDR"]=="127.0.0.1")){ 
  define("SQL_HOST","localhost");
  define("SQL_DBNAME","Pila");
  define("SQL_USERNAME","root");
  define("SQL_PASSWORD","moje.heslo");
  define("MD5_PASSWORD","moje.heslo");
  
}
else{ 
  define("SQL_HOST","localhost");
  define("SQL_DBNAME","Pila");
  define("SQL_USERNAME","root");
  define("SQL_PASSWORD","moje.heslo");
  define("MD5_PASSWORD","moje.heslo");
}
?>
