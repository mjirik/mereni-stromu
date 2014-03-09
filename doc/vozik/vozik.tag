<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>Zařízení na vozíku</title>
    <filename>index</filename>
    <docanchor file="index">vozik_rezervy</docanchor>
    <docanchor file="index">vozik_hardware</docanchor>
  </compound>
  <compound kind="file">
    <name>author.txt</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/docsrc/</path>
    <filename>author_8txt</filename>
  </compound>
  <compound kind="file">
    <name>buttons.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>buttons_8c</filename>
    <includes id="buttons_8h" name="buttons.h" local="yes" imported="no">buttons.h</includes>
    <includes id="keyboard_8h" name="keyboard.h" local="yes" imported="no">keyboard.h</includes>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>button_on</name>
      <anchorfile>buttons_8c.html</anchorfile>
      <anchor>acd06a8377fd0f71bed87188db5c904c3</anchor>
      <arglist>(char button)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>buttons_init</name>
      <anchorfile>buttons_8c.html</anchorfile>
      <anchor>acef6dd444cb6560da652897ee43ab306</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>buttons</name>
      <anchorfile>buttons_8c.html</anchorfile>
      <anchor>a82b6a0bb2d8a5f28c7be78d77f07a471</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>minuly_stav</name>
      <anchorfile>buttons_8c.html</anchorfile>
      <anchor>afd49bb358edddbdb2baf13667c693378</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>buttons.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>buttons_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>buttons</name>
      <anchorfile>buttons_8h.html</anchorfile>
      <anchor>a82b6a0bb2d8a5f28c7be78d77f07a471</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>buttons_init</name>
      <anchorfile>buttons_8h.html</anchorfile>
      <anchor>acef6dd444cb6560da652897ee43ab306</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>convert.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>convert_8c</filename>
    <includes id="convert_8h" name="convert.h" local="yes" imported="no">convert.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>mezery</name>
      <anchorfile>convert_8c.html</anchorfile>
      <anchor>a39b535915350e4ef7042813898302441</anchor>
      <arglist>(char *s, char len)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>strlen</name>
      <anchorfile>convert_8c.html</anchorfile>
      <anchor>aae9bab51ad4377373a2d4e9f7bbf0f2e</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>int2str</name>
      <anchorfile>convert_8c.html</anchorfile>
      <anchor>a74ec62624e7d8e4b00205adf77aabb64</anchor>
      <arglist>(char *s, uint16_t cislo)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>int2strn</name>
      <anchorfile>convert_8c.html</anchorfile>
      <anchor>a17bf3fd114d2e811eb0193d03398cf7a</anchor>
      <arglist>(char *s, int len, uint16_t cislo)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>convert.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>convert_8h</filename>
    <member kind="function">
      <type>char *</type>
      <name>int2str</name>
      <anchorfile>convert_8h.html</anchorfile>
      <anchor>a74868bf442139439da976b90c8277113</anchor>
      <arglist>(char *s, uint16_t i)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>int2strn</name>
      <anchorfile>convert_8h.html</anchorfile>
      <anchor>a0fdc20d175646f12ed735b0749ce9b01</anchor>
      <arglist>(char *s, int n, uint16_t i)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>mezery</name>
      <anchorfile>convert_8h.html</anchorfile>
      <anchor>a39b535915350e4ef7042813898302441</anchor>
      <arglist>(char *s, char len)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>default_scr.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>default__scr_8c</filename>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="irc2_8h" name="irc2.h" local="yes" imported="no">irc2.h</includes>
    <includes id="default__scr_8h" name="default_scr.h" local="yes" imported="no">default_scr.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="convert_8h" name="convert.h" local="yes" imported="no">convert.h</includes>
    <member kind="function">
      <type>void</type>
      <name>default_scr_draw</name>
      <anchorfile>default__scr_8c.html</anchorfile>
      <anchor>a92943a1d6c25c652c6fcc1ba8b60861a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>default_scr_control</name>
      <anchorfile>default__scr_8c.html</anchorfile>
      <anchor>a959dd0c7a626e32bb9328e93fee4278d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char s_zakz[]</type>
      <name>PROGMEM</name>
      <anchorfile>default__scr_8c.html</anchorfile>
      <anchor>ac595d68c2f45e2f775e81156c2d42850</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>default_scr.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>default__scr_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>default_scr_draw</name>
      <anchorfile>default__scr_8h.html</anchorfile>
      <anchor>a92943a1d6c25c652c6fcc1ba8b60861a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>default_scr_control</name>
      <anchorfile>default__scr_8h.html</anchorfile>
      <anchor>a959dd0c7a626e32bb9328e93fee4278d</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>doc.txt</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/docsrc/</path>
    <filename>doc_8txt</filename>
  </compound>
  <compound kind="file">
    <name>eeprom.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>eeprom_8c</filename>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="watchdog_8h" name="watchdog.h" local="yes" imported="no">watchdog.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DEFAULT_irc_k</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>a3fbd02c4db4c71e63711f57388545958</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFAULT_irc_set_on_value</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>a9f3f07cb2927c0e7a7af78cb259f00da</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_load</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>ac2c42f2dcbb794e911c7640d3fe24bb6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_save</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>a967222c6b1a76007b1656eca35ff7c08</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w8</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>ab66d92098db926da6dd7f28c9886d884</anchor>
      <arglist>(void *uiAddress, uint8_t c)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>eeprom_r8</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>aaf8e0160a22f011071c6eef05108dd02</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w16</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>aacd26e87298620ea075952631cdfe66a</anchor>
      <arglist>(void *uiAddress, uint16_t word)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>eeprom_r16</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>abe13ae00e4d18d600c9fb3b007f33a40</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w32</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>af83bc20fbdc5083109537fd2db28fa13</anchor>
      <arglist>(void *uiAddress, uint32_t data)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>eeprom_r32</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>a7879f50820ed839b1a3f83c3e6d5294d</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>eeprom.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>eeprom_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_counter_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>abc14ad542a25cb181424ac5129cabc66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_k_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a9b87dadec373be2f2c4b7a372a9d7d02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_set_on_value_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a2e817c573e3752b1a87da64339692358</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_width_src_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a0278d146c749aace1329b06ab183fb4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_flag_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>aaed292bd7b9e8d197d31f57c95c36c85</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_lst_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>aeb95ebd6bd902b30e4380b707537f57c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_data_size</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a4737983a3c375cf206f43a824e86c954</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_counter</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a7e7f16f7f5faef1189e2c269718a8366</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_k</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>ab448aa04f20455aba896df38be5e0c7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_set_on_value</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a38dc747d14591ede830e78c5f2ae6440</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_width_src</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>ad22ea911935c9edc5c6debab5abc3b6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_flag</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>af950e674ba924e46f868f0e0e466dafa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_lst</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a46ea9e83c7b9a3440d201e9d7b82b3b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_data</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>abf76ff63503b998e2fadf9dc86df16c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_load</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>ac2c42f2dcbb794e911c7640d3fe24bb6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_save</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a967222c6b1a76007b1656eca35ff7c08</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>eeprom_r8</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>aaf8e0160a22f011071c6eef05108dd02</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w8</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>ab66d92098db926da6dd7f28c9886d884</anchor>
      <arglist>(void *uiAddress, uint8_t c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w16</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>aacd26e87298620ea075952631cdfe66a</anchor>
      <arglist>(void *uiAddress, uint16_t word)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>eeprom_r16</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>abe13ae00e4d18d600c9fb3b007f33a40</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w32</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>af83bc20fbdc5083109537fd2db28fa13</anchor>
      <arglist>(void *uiAddress, uint32_t data)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>eeprom_r32</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a7879f50820ed839b1a3f83c3e6d5294d</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>width_src</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>a438d690afcf99fcf144f660af7e2cddc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>error.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>error_8c</filename>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="keyboard_8h" name="keyboard.h" local="yes" imported="no">keyboard.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="msgn_8h" name="msgn.h" local="yes" imported="no">msgn.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ERR_TIME</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a7f28a364345237e97db7906e251de307</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>error_text</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a72f3ba95376bd725543d9f73b14b1a62</anchor>
      <arglist>(uint8_t err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print_err</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>af263748d66d06bcc7e8cf3e8e5ddfb37</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_control</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>ad55f51b13448734dee621455a92e74f1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_scr_open</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a2dad9aca4b62c3ded6ee140807e1a9a5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_error</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>acb5cd57cbb1a2f9e632a003540b2df1d</anchor>
      <arglist>(uint8_t err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_str_error</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>af408b3e4a42f097577e775b58e78e06d</anchor>
      <arglist>(uint8_t err_num, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>error_scr_draw</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a7fc70ff887a46ce08c06b9b9762ef200</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>error_scr_control</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a316a9c1c700f5cde9d2fc570632a9f37</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_assert</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>ae6dd1f6c6a4f232ddee182df66ee0b28</anchor>
      <arglist>(char *soubor, int radek)</arglist>
    </member>
    <member kind="variable">
      <type>int8_t</type>
      <name>prev_scr</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>abcd96d5cf4202894aef292e65eea421e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>last_err</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a2852ad478aca7815ec04556c12023e01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>err_str</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a6472a0f347efd3e832db2b9f028d31a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>new_err</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a3730d7a244c803bf5c7536882cecf57e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char err1[]</type>
      <name>PROGMEM</name>
      <anchorfile>error_8c.html</anchorfile>
      <anchor>a46c9c313c59969496af3c9355cd5ecf1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>error.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>error_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>assert</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>ad6d5aaa966ca7424f7cb9bd01f2c838b</anchor>
      <arglist>(e)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_str_error</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>af408b3e4a42f097577e775b58e78e06d</anchor>
      <arglist>(uint8_t err_num, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_error</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>acb5cd57cbb1a2f9e632a003540b2df1d</anchor>
      <arglist>(uint8_t err_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>error_scr_draw</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>a7fc70ff887a46ce08c06b9b9762ef200</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>error_scr_control</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>a316a9c1c700f5cde9d2fc570632a9f37</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_assert</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>ae6dd1f6c6a4f232ddee182df66ee0b28</anchor>
      <arglist>(char *soubor, int radek)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_scr_open</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>a2dad9aca4b62c3ded6ee140807e1a9a5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err_control</name>
      <anchorfile>error_8h.html</anchorfile>
      <anchor>ad55f51b13448734dee621455a92e74f1</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>inout.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>inout_8c</filename>
    <includes id="inout_8h" name="inout.h" local="yes" imported="no">inout.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MSR_LED_PORT</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>a60c7c70e9a51aa1013666229b3b6f7aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSR_LED_DDR</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>ac6c2716ca080e960ea00fe2a60f2e5d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSR_LED_PIN</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>a098ccd35a6c0243e1e3a8d379d1a4279</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSR_LED_BIT</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>a89cea95871f81ab6e7b1581f6558b481</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inout_init</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>a4ae2f3f047db9172650ff84a543eb54e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_msr_led_on</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>a203f907fb83c309fe09d80666b7893d2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_msr_led_off</name>
      <anchorfile>inout_8c.html</anchorfile>
      <anchor>ac1d83154054f9bf4f80e8d38adf5bf42</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>inout.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>inout_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>io_msr_led_on</name>
      <anchorfile>inout_8h.html</anchorfile>
      <anchor>a203f907fb83c309fe09d80666b7893d2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>io_msr_led_off</name>
      <anchorfile>inout_8h.html</anchorfile>
      <anchor>ac1d83154054f9bf4f80e8d38adf5bf42</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>irc_8c</filename>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>A_PORT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>ac9baf768fde81ade6f9ccde1d20d5a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_PIN</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a955d6d7f3f806537072f993708e00f65</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_BIT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>ae59e66b2ffaf088f18f894088f53205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_PORT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a04e1bd382a6de1a30d813bb79f30fb02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_PIN</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a45bd909a8fb5205b60cd8c8d6760db7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_BIT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a07e5089fd757ebcad8929674eecf6c80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_PORT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a07104d9db9a8c1da66b457a777088113</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_PIN</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>acb5711bd7b75243461976cb414b29c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_BIT</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a2401c1030e02b8da7b3586e2f788d792</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc1_set_on_value</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a7e91f62bfba3c9a7d07bea319b0710f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>aa80a7e95233446dac8df1c9d8f4c263a</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc1_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a36575a375721a5efe36822edac5e7aec</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a478355ecd82c2b5e54b93297b402f95f</anchor>
      <arglist>(uint16_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_k_mm</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>afa0904827e1cdc1e33ae7de570216892</anchor>
      <arglist>(uint16_t i)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc1_get_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a72331a5215368486b6e9eb068e0ea1b1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_set_on_value_mm</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>afc8d405aa96a15f9304139867b024361</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_set_on_value</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a3515c748a2972e9aa03800b29050ee88</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc1_init</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>aa50caede5ea4ec03d972071a3ba60955</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc1_watch</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a1bce3e604934651f923c84b7fda5c6f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc1_get_l</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>acaa3b31cea039569b81247a457c0263e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned char</type>
      <name>prev_state</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>ad24a7209740a83b880691380fbe8e0e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int32_t</type>
      <name>irc_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a2420ea5087a6c97e0f0e60946bb6bed2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int32_t</type>
      <name>irc_set_on_value</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>ace11bc81992ca3824a4fb55ff1d5b47b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned char</type>
      <name>last_oper_with_irc_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a40b68af2e4f96a1cfd2785b6d3b8aa9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint16_t</type>
      <name>irc_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>a2823da0ac1f311c8b12078b236bbeb67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>irc_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>irc1_watch</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a1bce3e604934651f923c84b7fda5c6f6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_k_mm</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a34cbf3086c85ee2ee639c6e85feeb848</anchor>
      <arglist>(uint16_t)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc1_get_l</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>acaa3b31cea039569b81247a457c0263e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_set_on_value_mm</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>afc8d405aa96a15f9304139867b024361</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc1_get_k</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a72331a5215368486b6e9eb068e0ea1b1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc1_init</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>aa50caede5ea4ec03d972071a3ba60955</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc1_set_on_value</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a96221cc1f6e45c778e3909ff5d860b2d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_counter</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>aa80a7e95233446dac8df1c9d8f4c263a</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc1_counter</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a36575a375721a5efe36822edac5e7aec</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_k</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a478355ecd82c2b5e54b93297b402f95f</anchor>
      <arglist>(uint16_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc1_set_on_value</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>a3515c748a2972e9aa03800b29050ee88</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc2.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>irc2_8c</filename>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="irc2_8h" name="irc2.h" local="yes" imported="no">irc2.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>A_PORT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>ac9baf768fde81ade6f9ccde1d20d5a32</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_PIN</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a955d6d7f3f806537072f993708e00f65</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>A_BIT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>ae59e66b2ffaf088f18f894088f53205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_PORT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a04e1bd382a6de1a30d813bb79f30fb02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_PIN</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a45bd909a8fb5205b60cd8c8d6760db7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_BIT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a07e5089fd757ebcad8929674eecf6c80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_PORT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a07104d9db9a8c1da66b457a777088113</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_PIN</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>acb5711bd7b75243461976cb414b29c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SP_BIT</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a2401c1030e02b8da7b3586e2f788d792</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc2_set_on_value</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>aeae106a4dc2959c7a8e13ccdb9a43d95</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_counter</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>abe8163b2ba712f1a83891ce75e903037</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc2_counter</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a12bd5631a35458163e675a4738df8ac7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_k</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>aca073e77760684114a3d1f46adef8efc</anchor>
      <arglist>(uint16_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_k_mm</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a20d90a499e8980683f9c54225e397a9f</anchor>
      <arglist>(uint16_t i)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc2_get_k</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a8a8e13b00bbf1550a74a1aa2fd637470</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_set_on_value_mm</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a9d85c077b39fc2e091d54dcbfff72608</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_set_on_value</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a41a0bca6c07f8b1245a4b59a8209fce9</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc2_init</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a82d08977a4cf71b6cad65346a58ec837</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc2_watch</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>ac24ca6f0906ccdfb6ffe65d2a28abaf7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc2_get_l</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a9a7f216fac24f1bb5342700bdfcc865a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned char</type>
      <name>prev_state</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>ad24a7209740a83b880691380fbe8e0e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int32_t</type>
      <name>irc_counter</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a2420ea5087a6c97e0f0e60946bb6bed2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int32_t</type>
      <name>irc_set_on_value</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>ace11bc81992ca3824a4fb55ff1d5b47b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned char</type>
      <name>last_oper_with_irc_counter</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a40b68af2e4f96a1cfd2785b6d3b8aa9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint16_t</type>
      <name>irc_k</name>
      <anchorfile>irc2_8c.html</anchorfile>
      <anchor>a2823da0ac1f311c8b12078b236bbeb67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc2.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>irc2_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>irc2_watch</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>ac24ca6f0906ccdfb6ffe65d2a28abaf7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_k_mm</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a06a421e66d3fa4bc4991b708986beb49</anchor>
      <arglist>(uint16_t)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc2_get_l</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a9a7f216fac24f1bb5342700bdfcc865a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_set_on_value_mm</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a9d85c077b39fc2e091d54dcbfff72608</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc2_get_k</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a8a8e13b00bbf1550a74a1aa2fd637470</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc2_init</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a82d08977a4cf71b6cad65346a58ec837</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc2_set_on_value</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a5c251d5e2c3b9f72e291eada25902d54</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_counter</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>abe8163b2ba712f1a83891ce75e903037</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>get_irc2_counter</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a12bd5631a35458163e675a4738df8ac7</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_k</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>aca073e77760684114a3d1f46adef8efc</anchor>
      <arglist>(uint16_t a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc2_set_on_value</name>
      <anchorfile>irc2_8h.html</anchorfile>
      <anchor>a41a0bca6c07f8b1245a4b59a8209fce9</anchor>
      <arglist>(int32_t a)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>keyboard.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>keyboard_8c</filename>
    <includes id="keyboard_8h" name="keyboard.h" local="yes" imported="no">keyboard.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="convert_8h" name="convert.h" local="yes" imported="no">convert.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="msgn_8h" name="msgn.h" local="yes" imported="no">msgn.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory.h</includes>
    <class kind="struct">key</class>
    <member kind="define">
      <type>#define</type>
      <name>KEYB_RECV_WATCH_N</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>af82dee349f6ba70158ae157021bdc5b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>KEYB_BUFF_SIZE</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>af9b6f546f2720d1aabcaf9f1a8ad9b75</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct key</type>
      <name>KEY</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a38d80b67cca10d0f2c0a1706d7b63660</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zpracuj_kod</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a886e9a16dadc5bd9d5331a1ac3c028ae</anchor>
      <arglist>(char)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static KEY</type>
      <name>get_key_from_buff</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a634e36eab86a3bc50aee821b3770742e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyb_in_buff</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a4b4fdc8611d94ae6541af55ff1c7140f</anchor>
      <arglist>(char flag, char kod)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a1ac4291762bc07b651eae702323894fb</anchor>
      <arglist>(SIG_INTERRUPT1)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>get_char</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>af0d08de095c0c6a062d084562f436b9e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>get_spec</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a6a0ab525eb476112b5b6259dd180cfa9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyboard_init</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a5aae8263abc1dec4671638e7ed4af681</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyboard_recv_watchdog</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a01edeb550d4a5e49474b974d03adf41e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int8_t</type>
      <name>data_counter</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a94962cd5e2de8fc881ffdac88fe81449</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>recivedc</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a89959d68e404325764357ceceab9b4e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>minuly_kod</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>ac879afe8e9548108268fe0d1360e58a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static uint16_t</type>
      <name>keyb_recv_watchdog</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>a0231eef69075ddf53fe8c05bce81235a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static KEY</type>
      <name>keyb_buff</name>
      <anchorfile>keyboard_8c.html</anchorfile>
      <anchor>ac524b19ad362e0fe206ce5b3dae2e2fa</anchor>
      <arglist>[KEYB_BUFF_SIZE]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>keyboard.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>keyboard_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>keyboard_init</name>
      <anchorfile>keyboard_8h.html</anchorfile>
      <anchor>a5aae8263abc1dec4671638e7ed4af681</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>get_char</name>
      <anchorfile>keyboard_8h.html</anchorfile>
      <anchor>af0d08de095c0c6a062d084562f436b9e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>get_spec</name>
      <anchorfile>keyboard_8h.html</anchorfile>
      <anchor>a6a0ab525eb476112b5b6259dd180cfa9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyboard_recv_watchdog</name>
      <anchorfile>keyboard_8h.html</anchorfile>
      <anchor>a01edeb550d4a5e49474b974d03adf41e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>keyb_in_buff</name>
      <anchorfile>keyboard_8h.html</anchorfile>
      <anchor>a4b4fdc8611d94ae6541af55ff1c7140f</anchor>
      <arglist>(char flag, char kod)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>lcd.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>lcd_8c</filename>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="convert_8h" name="convert.h" local="yes" imported="no">convert.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>F_CPU</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a43bafb28b29491ec7f871319b5a3b2f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>lcd_new_frame</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a2fdcaa04d2a5ef5f829a2c0503b95009</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>d10</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>ae9bb5305a03d43b839016fcc98dfee4f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear_dscreen</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>af632fc4d520fbb4468baea39b68bc17e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear_screen</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a0d72a8b3fb4557babb0f1c61018f2327</anchor>
      <arglist>(char(*screen)[LCD_X_MAX])</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printc</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a7ead26e52916805b53cb705997cb90bb</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printnt_P</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>aa4fa8c7956bc27eae6a818d41199691f</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a51283220f829206d8ef3284047a58fbd</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printcxyd</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>ab0348002a81c8fe945f405d57b317db1</anchor>
      <arglist>(int y, int x, char c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printnxyd</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a9b70b5f13bd19a1a5ff3de4221c1dc72</anchor>
      <arglist>(int y, int x, char *s, int n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxyd</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>af14cee58cda35b9331b261be186b3dd9</anchor>
      <arglist>(int y, int x, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxy</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a4aaab14a959b518f3d8191174a94b485</anchor>
      <arglist>(char(*screen)[LCD_X_MAX], int y, int x, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxyd_P</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>ac49e278f3e1c0c3472a70b8c6efc5051</anchor>
      <arglist>(int y, int x, void *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_w_inst</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a98e19b75c7263dd914e04a630f43cde1</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_init</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a6842775ba83d166f02b8fef8bb63b1e6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_control</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>aac5a8ac84d029ba77193f158761d10da</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>lcd_ctrl_count</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a1fabb9483b10918d452fee90b071d54c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>lcd_ctrl_x</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>a1cc294e76003b0f360663bab284352bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>lcd_ctrl_y</name>
      <anchorfile>lcd_8c.html</anchorfile>
      <anchor>ab5728eedb69611680d9b2e95c449bea2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>lcd.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>lcd_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>LCD_X_MAX</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a1d9ba2d8f9cc2124ff29e24abbe2c90d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LCD_Y_MAX</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>ad14583f7d91ea48ff9f3e6ec9a920a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DVA_E</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>aff26c292d8bc1fa5c6cf136b7f73b557</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_init</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a6842775ba83d166f02b8fef8bb63b1e6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_control</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>aac5a8ac84d029ba77193f158761d10da</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxy</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a753417a54b9788480a2d26716cde2a71</anchor>
      <arglist>(char(*screen)[LCD_X_MAX], int x, int y, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printc</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a7ead26e52916805b53cb705997cb90bb</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a51283220f829206d8ef3284047a58fbd</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear_screen</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a0d72a8b3fb4557babb0f1c61018f2327</anchor>
      <arglist>(char(*screen)[LCD_X_MAX])</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>lcd_new_frame</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a2fdcaa04d2a5ef5f829a2c0503b95009</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printcxyd</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a73a10b7ac5330a420af6e1f443506937</anchor>
      <arglist>(int x, int y, char c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printnxyd</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a9b70b5f13bd19a1a5ff3de4221c1dc72</anchor>
      <arglist>(int y, int x, char *s, int n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxyd</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>af14cee58cda35b9331b261be186b3dd9</anchor>
      <arglist>(int y, int x, char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear_dscreen</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>af632fc4d520fbb4468baea39b68bc17e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printxyd_P</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>ac49e278f3e1c0c3472a70b8c6efc5051</anchor>
      <arglist>(int y, int x, void *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printnt_P</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>aa4fa8c7956bc27eae6a818d41199691f</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lcd_w_inst</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a98e19b75c7263dd914e04a630f43cde1</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="variable">
      <type>char(*</type>
      <name>p_akt_screen</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a29229a718c28cd99acb4cb22eed55c33</anchor>
      <arglist>)[LCD_X_MAX]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>default_screen</name>
      <anchorfile>lcd_8h.html</anchorfile>
      <anchor>a461f2d625bd30828c33677c4c500f3a2</anchor>
      <arglist>[LCD_Y_MAX][LCD_X_MAX]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>main.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>main_8c</filename>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="irc2_8h" name="irc2.h" local="yes" imported="no">irc2.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="convert_8h" name="convert.h" local="yes" imported="no">convert.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <includes id="keyboard_8h" name="keyboard.h" local="yes" imported="no">keyboard.h</includes>
    <includes id="buttons_8h" name="buttons.h" local="yes" imported="no">buttons.h</includes>
    <includes id="watchdog_8h" name="watchdog.h" local="yes" imported="no">watchdog.h</includes>
    <includes id="msgn_8h" name="msgn.h" local="yes" imported="no">msgn.h</includes>
    <includes id="inout_8h" name="inout.h" local="yes" imported="no">inout.h</includes>
    <includes id="default__scr_8h" name="default_scr.h" local="yes" imported="no">default_scr.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>F_CPU</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a43bafb28b29491ec7f871319b5a3b2f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T_LCD_MAX</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a19cebd54c6081dbcf41461843a890cf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>get_data_from_katr</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a28f3bbd07c7cbecfc65e4ac3ecee521b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_irc_values_to_PC</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a6de58cf94dae7ef363eccf43754a3561</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_used_mem</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>ad95535453dafe59b5938deeec23ababc</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_shutdown</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a69f95b1ec6b9c81ade176f604a9b2034</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>acc0188f99a5e4cb2df4cf3d81fc373ef</anchor>
      <arglist>(SIG_OUTPUT_COMPARE1A)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ioinit</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>af63a4042de4220d64088f03ef65f9dbf</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a840291bc02cba5474a4cb46a9b9566fe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>getstr_P</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a94c5fb482b0b9ce39cae1619bf51b0ae</anchor>
      <arglist>(PGM_P s_p)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>prazdna</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>af5109660bca826e0961aef144489df25</anchor>
      <arglist>(char *s)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>get_data_from_server</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a05ef6c172afbefcf3a1516b7b264512e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>err102</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>ad923ea58f0b1a424eed0136b63b555ee</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>t_lcd</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>a4d08c8e52670a496e032b874c807de57</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char err98[]</type>
      <name>PROGMEM</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>aba575ae902f939508a51e3391492f700</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>main.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>main_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>SizeOfMsg</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>a8639f3cdce498b36b4f11b82726f3857</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_TICTOC</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>a29835c64ae960bf66a163842a562abde</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>getstr_P</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>a94c5fb482b0b9ce39cae1619bf51b0ae</anchor>
      <arglist>(PGM_P s_p)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>my_watchdog</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>acf97371d95f45fa044173f164fec85f1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>tic</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>ace51fd7d8435f88c8b2b3bcf64367673</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>toc</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>a88df1dabdebb9698dcabfeb1359fa23f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>str_buf</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>a7ae8aac9d74f11560f00e9f0a396b264</anchor>
      <arglist>[41]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>manual.txt</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/docsrc/</path>
    <filename>manual_8txt</filename>
  </compound>
  <compound kind="file">
    <name>memory.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>memory_8c</filename>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <member kind="function">
      <type>void *</type>
      <name>mmalloc</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>ac1847e839d655a069b4c77d164f395c5</anchor>
      <arglist>(unsigned int size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mfree</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>a9ba414662a250ef92d4296a19dafad43</anchor>
      <arglist>(void *pointer)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>used_mem</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>a8dae36488b9df941d0d6734093c03c1f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>used_mem_blocks</name>
      <anchorfile>memory_8c.html</anchorfile>
      <anchor>ad1acea3e104f63e81b2e128dc46222d9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>memory.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>memory_8h</filename>
    <member kind="function">
      <type>void *</type>
      <name>mmalloc</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>ac1847e839d655a069b4c77d164f395c5</anchor>
      <arglist>(unsigned int size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mfree</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>a9ba414662a250ef92d4296a19dafad43</anchor>
      <arglist>(void *pointer)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>used_mem</name>
      <anchorfile>memory_8h.html</anchorfile>
      <anchor>a8dae36488b9df941d0d6734093c03c1f</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>msgn.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>msgn_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_KEY</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>adb5a684abf984a7c4a304b6becb527eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_IRC</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a4ca50e2e3bf7070065a13fc25e02af62</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_ERR</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a256f1e536f011465dc976de883b7b65f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD00</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>aff9f0f94101631ede014778e299b3879</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD01</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>af116111a265b4660f4ffd6daf5974b34</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD02</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>ad6ae28419e1317eda5cc3dc4c76f3155</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD03</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a8133c0a21992969dd8196478742764da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD04</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>acf76b82d8ef6c6c96dd8a7dfb89083c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD05</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>aff39c62d7ad1fee68c450d0577339a63</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD06</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a5a94abc4b3c41f5921f780a0c32dfca9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD07</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>ae509d1d47cb2758657c557a820c2fecb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD08</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a3ed951f14b88350a3a5476d305b4af99</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD09</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a2803311599005c011b6b9e036fb379e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD10</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>ad65840531e7a4be28294031c2c369f67</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD11</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a5dfd650f9326e146b9fb448299619f85</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD12</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>aae9a4f3b8fc7449c7b89e211d0339401</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD13</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>aa7f1e81a361a46e66e3fe05027006de8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD14</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a6c24d17bb347692fed46a19548bfa200</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_LCD15</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a1b5cf94e3f3fa3cb5d4a14c33b880809</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_MSR_LED_ON</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>a65d893eb7a82a4d1d27565e7ded27d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MSGN_MSR_LED_OFF</name>
      <anchorfile>msgn_8h.html</anchorfile>
      <anchor>ae93b6ba9db193290fead6f1a566b84eb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>usart_8c</filename>
    <includes id="usart__recv_8h" name="usart_recv.h" local="yes" imported="no">usart_recv.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="msgn_8h" name="msgn.h" local="yes" imported="no">msgn.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>LenOfMsgBuffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a1bff58c0bf01ec2912bb7138fbb7fdad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Delay</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ada5117fc4f5cae9ee7091b2fd013ad02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_MSG_ACK_DELAY</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a29d336459d4cafff07606c0f9cb09017</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zarad_tri_zpravy</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a5a2865727ce58839815fd664d3c376fd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a759190db192ee81162c88decd75d3a7d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>test_message</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a2a0c93037e5ab2c9fdb185a48ce7b2f9</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process_recived_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a060e86a97604dcd6e6a50b5ec98fa364</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reset_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>af3ae067d06a9dbe11d072611e842f4e8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>into_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aa442f9e21b290cd042db309fd0499ebc</anchor>
      <arglist>(unsigned char c)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>read_port</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a5b29ec52b36ee832b285df1152da85b8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>USART_Init</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ac6ebeb4ed2c70b120906228519a9c246</anchor>
      <arglist>(unsigned int baud)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_time_pp</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a170941d619f0e1bbf13ec95111cecb19</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_time_0</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a0405ce6027c0b29fe71ec8a995df096f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>get_msg_time</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ac615ce8c07d61bbd036d865021d0535b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aad0b06468af935e89ae3f73ee25a2d94</anchor>
      <arglist>(SIG_UART_RECV)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a385355aaf5ae15bdaa3a7d44f2fb4dae</anchor>
      <arglist>(SIG_UART_DATA)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>to_transmitt_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aef360141033e4d40aaacbfbbf3d4f187</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a738f7989737ea5e614ea10d16eb70004</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg_P</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a2da7b3916466fa7fbf44b68406ee9b76</anchor>
      <arglist>(PGM_P s_p)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_ack_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aece32bc3ab3397ea8202203edf547eb2</anchor>
      <arglist>(char *msg, unsigned char repeats, void(*p_fcn)(char *s), void(*p_fcn_nresponse)(void))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_ack_fcn</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a2c55776d951e70bb73e5b15621e30361</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_nack_fcn</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ad9ac0375c30caaefc85d21ba2a869fff</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serial_msg_ack_timer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aeeeece096429d4491b2992685be8f0c6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>usart_get_recv_buff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a5ac8e318d028590a813919560a120742</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_uint16</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aae4c07a844ed5fbcb7284bc9cd8b2831</anchor>
      <arglist>(char msgn, uint16_t number1, uint16_t number2)</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>usart_recv_count</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a3bb63f7d4ccc2ae47270f9ee0a86f8bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>usart_recv_buff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>afa759eaccfefd87bdc2bf4235d2e3e6b</anchor>
      <arglist>[LenOfRecvBuff]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>usart_trans_buff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ade3e12ab8f79f2d6de185c3debfe47ef</anchor>
      <arglist>[LenOfTransBuff]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>prepinani</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>af0117ac5b82c30815543978d5fcdd10c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>usart_trans_count</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>abd954e0bb7aa2f9b561bbc55c24a4838</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>msg_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aa564ee2888e0a4d5e18727d14778b096</anchor>
      <arglist>[LenOfMsgBuffer]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>msg_buffer_counter</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ac24254f988d7e9b7904c19993cb0b866</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>recived_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a21394a6033cbdcabb80cb7c1b43fde38</anchor>
      <arglist>[LenOfMsg]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>after_trans_delay_counter</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a7639023156bfa7e13b6d2f8c0a59d5d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned int</type>
      <name>msg_ack_timer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a4d78700644f7a01d4990efb37c3c0d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char</type>
      <name>last_msg_ack</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ad90b59d623b0852bbe7186b176930fd4</anchor>
      <arglist>[LenOfTransBuff]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static unsigned char</type>
      <name>msg_ack_repeats</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a6618eaa5febd19bb1c7b2aacdaf839cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static void(*</type>
      <name>p_ack_fcn</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>aa1c9eb028377741d4020e05bd800a26a</anchor>
      <arglist>)(char *s)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static void(*</type>
      <name>p_nack_fcn</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>afb5822d7b07aae092e00adaa5e99668e</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>msg_time</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a35cefd336a24746f2d8203c9fabf54c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>usart_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>DATASIZE</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>acb383144a04ad304680b840cd65af5a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfMsg</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a3225b26e3c8329d83e786c441799c2d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfRecvBuff</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a917d375e0dd558c0164221a1a0a75a97</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfTransBuff</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>ad24ac1262ed4066d12091ee2d4b640ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LCD_NBYTES</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>ae86b88ffc9a31c1fea5b564c0aadbadb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>USART_Init</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>ac6ebeb4ed2c70b120906228519a9c246</anchor>
      <arglist>(unsigned int baud)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>af7f7e8050594c5346c22172e19d3c193</anchor>
      <arglist>(char *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_ack_msg</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>aece32bc3ab3397ea8202203edf547eb2</anchor>
      <arglist>(char *msg, unsigned char repeats, void(*p_fcn)(char *s), void(*p_fcn_nresponse)(void))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_ack_fcn</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a2c55776d951e70bb73e5b15621e30361</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>serial_msg_ack_timer</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>aeeeece096429d4491b2992685be8f0c6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg_P</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a2da7b3916466fa7fbf44b68406ee9b76</anchor>
      <arglist>(PGM_P s_p)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>usart_get_recv_buff</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a5ac8e318d028590a813919560a120742</anchor>
      <arglist>(int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_uint16</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>aae4c07a844ed5fbcb7284bc9cd8b2831</anchor>
      <arglist>(char msgn, uint16_t number1, uint16_t number2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_time_0</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a0405ce6027c0b29fe71ec8a995df096f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_time_pp</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a170941d619f0e1bbf13ec95111cecb19</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>get_msg_time</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>ac615ce8c07d61bbd036d865021d0535b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>msg_time</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>a35cefd336a24746f2d8203c9fabf54c6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart_recv.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>usart__recv_8c</filename>
    <includes id="usart__recv_8h" name="usart_recv.h" local="yes" imported="no">usart_recv.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="error_8h" name="error.h" local="yes" imported="no">error.h</includes>
    <includes id="msgn_8h" name="msgn.h" local="yes" imported="no">msgn.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="inout_8h" name="inout.h" local="yes" imported="no">inout.h</includes>
    <includes id="memory_8h" name="memory.h" local="yes" imported="no">memory.h</includes>
    <member kind="function">
      <type>void</type>
      <name>recv_msg</name>
      <anchorfile>usart__recv_8c.html</anchorfile>
      <anchor>a8c511fa0fa24a5d675b8142ffc483ecb</anchor>
      <arglist>(char *recived_msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_0A</name>
      <anchorfile>usart__recv_8c.html</anchorfile>
      <anchor>ad3b266182705026567b1a68e620045c2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_14</name>
      <anchorfile>usart__recv_8c.html</anchorfile>
      <anchor>a87d414e7cbc9fdef2f5e9e1e645dc7e5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char msg0A[LenOfMsg+1]</type>
      <name>PROGMEM</name>
      <anchorfile>usart__recv_8c.html</anchorfile>
      <anchor>aa52975c2383dde8754296111731357e0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart_recv.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>usart__recv_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>recv_msg</name>
      <anchorfile>usart__recv_8h.html</anchorfile>
      <anchor>a8c511fa0fa24a5d675b8142ffc483ecb</anchor>
      <arglist>(char *recived_msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_0A</name>
      <anchorfile>usart__recv_8h.html</anchorfile>
      <anchor>ad3b266182705026567b1a68e620045c2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>msg_14</name>
      <anchorfile>usart__recv_8h.html</anchorfile>
      <anchor>a87d414e7cbc9fdef2f5e9e1e645dc7e5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>width_src</name>
      <anchorfile>usart__recv_8h.html</anchorfile>
      <anchor>a438d690afcf99fcf144f660af7e2cddc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>watchdog.c</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>watchdog_8c</filename>
    <includes id="watchdog_8h" name="watchdog.h" local="yes" imported="no">watchdog.h</includes>
    <includes id="lcd_8h" name="lcd.h" local="yes" imported="no">lcd.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MY_WATCHDOG</name>
      <anchorfile>watchdog_8c.html</anchorfile>
      <anchor>a1ec06ff01e8ca7e4f7c2a673ba1ac499</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>wtch_init</name>
      <anchorfile>watchdog_8c.html</anchorfile>
      <anchor>acbdab9cad3f2f8b16ff00ad69af20145</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>wtch_main</name>
      <anchorfile>watchdog_8c.html</anchorfile>
      <anchor>ab51323d0a8ec8c832d7be1ab44ffbc3b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static char err97[]</type>
      <name>PROGMEM</name>
      <anchorfile>watchdog_8c.html</anchorfile>
      <anchor>a82348d6937f5354d885236312473f51f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>wtch_counter</name>
      <anchorfile>watchdog_8c.html</anchorfile>
      <anchor>a65366c710bff228f8a652480055368bc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>watchdog.h</name>
    <path>D:/Dokumenty/My Dropbox/projects/mereni_stromu/hardware/vozik_c/</path>
    <filename>watchdog_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>wtch_reset</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>a838ac15df7e81187dbe205888842548c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wtch_reset2</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>aec7bb1ec2da2c0cd4a620e8201652167</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wtch_disable2</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>a7a047399f54a574f9f9c6c477cc1cdd5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wtch_enable2</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>ab6f29e0682cc95138fa8de612c529b43</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>wtch_main</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>ab51323d0a8ec8c832d7be1ab44ffbc3b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>wtch_init</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>acbdab9cad3f2f8b16ff00ad69af20145</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>wtch_counter</name>
      <anchorfile>watchdog_8h.html</anchorfile>
      <anchor>a65366c710bff228f8a652480055368bc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>Manual</name>
    <title>Manuál obsluhy</title>
    <filename>Manual</filename>
    <docanchor file="Manual">obrazovka_data</docanchor>
    <docanchor file="Manual">obrazovka_mereni</docanchor>
    <docanchor file="Manual">uvod</docanchor>
    <docanchor file="Manual">obrazovka_setup</docanchor>
    <docanchor file="Manual">obrazovka_serial</docanchor>
    <docanchor file="Manual">obsluzna_jednotka</docanchor>
    <docanchor file="Manual">proces_mereni</docanchor>
    <docanchor file="Manual">obrazovka_zakaznici</docanchor>
  </compound>
  <compound kind="struct">
    <name>key</name>
    <filename>structkey.html</filename>
    <member kind="variable">
      <type>char</type>
      <name>flag</name>
      <anchorfile>structkey.html</anchorfile>
      <anchor>ae913bbd98681f233b7fb276d3b30500d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>kod</name>
      <anchorfile>structkey.html</anchorfile>
      <anchor>a5e1fd3055f0a412982436335356bb377</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
