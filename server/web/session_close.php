<?php
// $Id: session_close.php 51 2007-08-02 09:44:53Z Mira $
/**
 * @file session_close.php
 * Skript uzavírá session a odhlašuje uživatele .
 *  
 * @todo Odladit funkci odhlašování  
 */
session_destroy();
session_unset();
include ("login.php");

?>
