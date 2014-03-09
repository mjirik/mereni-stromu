<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>eeprom.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>eeprom_8c</filename>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DEFAULT_irc_k</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>3fbd02c4db4c71e63711f57388545958</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEFAULT_irc_set_on_value</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>9f3f07cb2927c0e7a7af78cb259f00da</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_load</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>c2c42f2dcbb794e911c7640d3fe24bb6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_save</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>967222c6b1a76007b1656eca35ff7c08</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w8</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>b66d92098db926da6dd7f28c9886d884</anchor>
      <arglist>(void *uiAddress, uint8_t c)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>eeprom_r8</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>af8e0160a22f011071c6eef05108dd02</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w16</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>acd26e87298620ea075952631cdfe66a</anchor>
      <arglist>(void *uiAddress, uint16_t word)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>eeprom_r16</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>be13ae00e4d18d600c9fb3b007f33a40</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w32</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>f83bc20fbdc5083109537fd2db28fa13</anchor>
      <arglist>(void *uiAddress, uint32_t data)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>eeprom_r32</name>
      <anchorfile>eeprom_8c.html</anchorfile>
      <anchor>7879f50820ed839b1a3f83c3e6d5294d</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>eeprom.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>eeprom_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_counter</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>7e7f16f7f5faef1189e2c269718a8366</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_k</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>b448aa04f20455aba896df38be5e0c7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ee_irc_set_on_value</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>38dc747d14591ede830e78c5f2ae6440</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_load</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>c2c42f2dcbb794e911c7640d3fe24bb6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_save</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>967222c6b1a76007b1656eca35ff7c08</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>eeprom_r8</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>af8e0160a22f011071c6eef05108dd02</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w8</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>b66d92098db926da6dd7f28c9886d884</anchor>
      <arglist>(void *uiAddress, uint8_t c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w16</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>acd26e87298620ea075952631cdfe66a</anchor>
      <arglist>(void *uiAddress, uint16_t word)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>eeprom_r16</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>be13ae00e4d18d600c9fb3b007f33a40</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_w32</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>f83bc20fbdc5083109537fd2db28fa13</anchor>
      <arglist>(void *uiAddress, uint32_t data)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>eeprom_r32</name>
      <anchorfile>eeprom_8h.html</anchorfile>
      <anchor>7879f50820ed839b1a3f83c3e6d5294d</anchor>
      <arglist>(void *uiAddress)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>irc_8c</filename>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="width_8h" name="width.h" local="yes" imported="no">width.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SizeOfMsg</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>8639f3cdce498b36b4f11b82726f3857</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc_get_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>92a83fe95657fd7f8829bfb26ab5dce4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc_get_d</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>cb7fb2dd68a049663856f3c58c5df035</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc_init</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>22c4d40478e2da08e616d92de5442378</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>36d5c4582e72f86bb243816590152bf1</anchor>
      <arglist>(uint16_t i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc_set_on_value</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>e8649f1f08cf3f0a6cc0d3e55eac5e21</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc_watch</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>bcea345e2f5f79d052b85a22d51ad123</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc_send_msg</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>2b23a703e44cfe694de1e8ad0e054638</anchor>
      <arglist>(char sure)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>prev_state</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>d24a7209740a83b880691380fbe8e0e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>irc_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>2420ea5087a6c97e0f0e60946bb6bed2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>irc_set_on_value</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>ce11bc81992ca3824a4fb55ff1d5b47b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>last_oper_with_irc_counter</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>40b68af2e4f96a1cfd2785b6d3b8aa9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>data_msg</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>644ab1e6fbad5b17be3f8141d1662682</anchor>
      <arglist>[SizeOfMsg]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>irc_dif_msg</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>d48a3158a9a9f6edd906b8866c818ea6</anchor>
      <arglist>[SizeOfMsg]</arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>irc_k</name>
      <anchorfile>irc_8c.html</anchorfile>
      <anchor>2823da0ac1f311c8b12078b236bbeb67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>irc.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>irc_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>irc_watch</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>bcea345e2f5f79d052b85a22d51ad123</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc_k</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>449f740513ef32944b53ceb5e64c7530</anchor>
      <arglist>(uint16_t)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc_get_d</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>cb7fb2dd68a049663856f3c58c5df035</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_irc_set_on_value</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>e8649f1f08cf3f0a6cc0d3e55eac5e21</anchor>
      <arglist>(int32_t q)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>irc_get_k</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>92a83fe95657fd7f8829bfb26ab5dce4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc_init</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>22c4d40478e2da08e616d92de5442378</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>irc_send_msg</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>2b23a703e44cfe694de1e8ad0e054638</anchor>
      <arglist>(char sure)</arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>irc_counter</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>2420ea5087a6c97e0f0e60946bb6bed2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32_t</type>
      <name>irc_set_on_value</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>ce11bc81992ca3824a4fb55ff1d5b47b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>irc_k</name>
      <anchorfile>irc_8h.html</anchorfile>
      <anchor>2823da0ac1f311c8b12078b236bbeb67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>main.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>main_8c</filename>
    <includes id="width_8h" name="width.h" local="yes" imported="no">width.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SizeOfMsg</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>8639f3cdce498b36b4f11b82726f3857</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>F_CPU</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>43bafb28b29491ec7f871319b5a3b2f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>cc0188f99a5e4cb2df4cf3d81fc373ef</anchor>
      <arglist>(SIG_OUTPUT_COMPARE1A)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_shutdown</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>69f95b1ec6b9c81ade176f604a9b2034</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ioinit</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>f63a4042de4220d64088f03ef65f9dbf</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>840291bc02cba5474a4cb46a9b9566fe</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>data_msg</name>
      <anchorfile>main_8c.html</anchorfile>
      <anchor>644ab1e6fbad5b17be3f8141d1662682</anchor>
      <arglist>[SizeOfMsg]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>main.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>main_8h</filename>
    <member kind="variable">
      <type>char</type>
      <name>data_msg</name>
      <anchorfile>main_8h.html</anchorfile>
      <anchor>764149ca67e94a94945b60d4c93863d0</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>usart_8c</filename>
    <includes id="usart__recv_8h" name="usart_recv.h" local="yes" imported="no">usart_recv.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DATASIZE</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>cb383144a04ad304680b840cd65af5a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfMsg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>3225b26e3c8329d83e786c441799c2d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfRecvBuff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>917d375e0dd558c0164221a1a0a75a97</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfTransBuff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>d24ac1262ed4066d12091ee2d4b640ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LenOfMsgBuffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>1bff58c0bf01ec2912bb7138fbb7fdad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Delay</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>da5117fc4f5cae9ee7091b2fd013ad02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>set_bit</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>6c162342474d34f8c1779594a389e823</anchor>
      <arglist>(c, bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>clear_bit</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>977e1538a0e8f01bd867a9c98cbaffb3</anchor>
      <arglist>(c, bit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>USART_Init</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>c6ebeb4ed2c70b120906228519a9c246</anchor>
      <arglist>(unsigned int baud)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>zarad_tri_zpravy</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>5a2865727ce58839815fd664d3c376fd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>send_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>759190db192ee81162c88decd75d3a7d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>f7f7e8050594c5346c22172e19d3c193</anchor>
      <arglist>(char *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>test_message</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>2a0c93037e5ab2c9fdb185a48ce7b2f9</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process_recived_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>de70a49b9ce25bcf4eee6655335388d3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>odesilame</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>1ff35b59737fc6dce1a47fbd8f878cfe</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ad0b06468af935e89ae3f73ee25a2d94</anchor>
      <arglist>(SIG_UART_RECV)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SIGNAL</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>385355aaf5ae15bdaa3a7d44f2fb4dae</anchor>
      <arglist>(SIG_UART_DATA)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>to_transmitt_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>ef360141033e4d40aaacbfbbf3d4f187</anchor>
      <arglist>(char *msg)</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>usart_recv_count</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>3bb63f7d4ccc2ae47270f9ee0a86f8bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>usart_recv_buff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>fa759eaccfefd87bdc2bf4235d2e3e6b</anchor>
      <arglist>[LenOfRecvBuff]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>usart_trans_buff</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>de3e12ab8f79f2d6de185c3debfe47ef</anchor>
      <arglist>[LenOfTransBuff]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>usart_trans_count</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>bd954e0bb7aa2f9b561bbc55c24a4838</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>msg_buffer</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a564ee2888e0a4d5e18727d14778b096</anchor>
      <arglist>[LenOfMsgBuffer]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>msg_buffer_counter</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>c24254f988d7e9b7904c19993cb0b866</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>recived_msg</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>21394a6033cbdcabb80cb7c1b43fde38</anchor>
      <arglist>[LenOfMsg]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>after_trans_delay_counter</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>7639023156bfa7e13b6d2f8c0a59d5d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>vysilame</name>
      <anchorfile>usart_8c.html</anchorfile>
      <anchor>a91da9d930bda2f4765850aca7f57b90</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>usart_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>USART_Init</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>c6ebeb4ed2c70b120906228519a9c246</anchor>
      <arglist>(unsigned int baud)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_msg</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>f7f7e8050594c5346c22172e19d3c193</anchor>
      <arglist>(char *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>odesilame</name>
      <anchorfile>usart_8h.html</anchorfile>
      <anchor>1ff35b59737fc6dce1a47fbd8f878cfe</anchor>
      <arglist>(char c)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart_recv.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>usart__recv_8c</filename>
    <includes id="usart__recv_8h" name="usart_recv.h" local="yes" imported="no">usart_recv.h</includes>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="eeprom_8h" name="eeprom.h" local="yes" imported="no">eeprom.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <includes id="width_8h" name="width.h" local="yes" imported="no">width.h</includes>
    <member kind="function">
      <type>void</type>
      <name>recv_msg</name>
      <anchorfile>usart__recv_8c.html</anchorfile>
      <anchor>8c511fa0fa24a5d675b8142ffc483ecb</anchor>
      <arglist>(char *recived_msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usart_recv.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>usart__recv_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>recv_msg</name>
      <anchorfile>usart__recv_8h.html</anchorfile>
      <anchor>8c511fa0fa24a5d675b8142ffc483ecb</anchor>
      <arglist>(char *recived_msg)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>width.c</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>width_8c</filename>
    <includes id="usart_8h" name="usart.h" local="yes" imported="no">usart.h</includes>
    <includes id="main_8h" name="main.h" local="yes" imported="no">main.h</includes>
    <includes id="irc_8h" name="irc.h" local="yes" imported="no">irc.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>LenOfWidIntegrArr</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>4c8223e37dcdb26f92b32dea5bcdd62e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>widthIterLimit</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>e0aefe8511ffe587f5579c715f827e2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>widthIterComparator</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>80c64b38ef18a72c2a34309cdd8a454d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SizeOfMsg</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>8639f3cdce498b36b4f11b82726f3857</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>widthSlow</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>8aa6e267df3a121a5d5187aa8b1e267a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>vzdalSvetlaOdSnimacu</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>00a06f68a27f566c37e5835b443b39f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>vzdalSnimacuOdStromu</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>3296643e72916022ad8fff528f9e3841</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_watch</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>a184476239f007137274f67a527e76a5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_watch_init</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>ca643d23675a5e4cb4db5d59a144544f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_computation</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>f7ca1de6284dbaba080478ffc9f35ed1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_send_msg</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>b8ba8ae00e137cd48d48089b44422a84</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>get_d_h</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>dc5e5de00526ee21858f22fd7d721b29</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>width_integrate_array</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>fb651afd0a08e3bb953b24f0e8a01aed</anchor>
      <arglist>[LenOfWidIntegrArr]</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>width_multiplex_adress</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>5733cce2e2a05d2e8300f0171a07982a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>width_counter</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>635eaca0e8a4201d47298f09ce0f4dea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>width_valid</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>fccd34e96fdbdbd5ac9091ccdec97940</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>width_value</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>f1fcc994df39613f5b36c3e3034f2a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>data_msg</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>644ab1e6fbad5b17be3f8141d1662682</anchor>
      <arglist>[SizeOfMsg]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>width_slow_counter</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>5c957366a7ea4d5eadc8f9dfd0b3ce7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>width_prepocet</name>
      <anchorfile>width_8c.html</anchorfile>
      <anchor>3724065d4ab6824238a5b10d8fef705d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>width.h</name>
    <path>D:/Dokumenty/projects/mereni_stromu/hardware/katr_c/</path>
    <filename>width_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>width_watch</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>a184476239f007137274f67a527e76a5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_watch_init</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>ca643d23675a5e4cb4db5d59a144544f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>width_computation</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>f7ca1de6284dbaba080478ffc9f35ed1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>w_slij</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>af961853a28e76b018b378a52af84708</anchor>
      <arglist>(char predchozi)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>get_d_h</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>dc5e5de00526ee21858f22fd7d721b29</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>width_prepocet</name>
      <anchorfile>width_8h.html</anchorfile>
      <anchor>3724065d4ab6824238a5b10d8fef705d</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
