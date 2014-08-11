set password = password ('moje.heslo');
drop database if exists Pila;
create database Pila;
use Pila;
source ./db/crebas.sql
source ./db/sp.sql
