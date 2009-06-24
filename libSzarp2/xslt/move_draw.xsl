<?xml version="1.0" encoding="ISO-8859-2"?>
<!-- 
  SZARP: SCADA software 
  

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
-->

<!--
	2003 Pawe� Pa�ucha PRATERM S.A. pawel@praterm.com.pl

	$Id: move_draw.xsl 4393 2007-09-13 16:37:10Z reksio $

	Ten szablon zwi�ksza wszystkim wybranym elementom 'draw' atrybut
	'prior' o zadan� liczb� (je�li go posiadaj�). Elementy 'draw' 
	s� wybierane na podstawie ci�gu zawartego w atrybucie 'title'.

	Parametry:
		title - ci�g zawarty w tytule wybieranych wykres�w
		num - liczba o jak� nale�y zwi�kszy� atrybut prior (je�li 
			nie podano, to 1), mo�e by� tak�e ujemna

	Przyk�ad u�ycia (zamienic '+' na '-'):
	# xsltproc ++stringparam title 'Al. Majowa 14' ++stringparam num 3 \
		params.xml
 -->
 
<xsl:stylesheet version="1.0" 
	xmlns:ipk="http://www.praterm.com.pl/SZARP/ipk"
	xmlns="http://www.praterm.com.pl/SZARP/ipk"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<!-- Deklaracja parametru 'list' -->
	<xsl:param name="num" value="1"/>
	<xsl:param name="title"/>

	<!-- Przetwarzamy ��dane elementy 'draw' -->
	<xsl:template match="ipk:param//ipk:draw[contains(@title,$title)]">
		<xsl:copy>
			<xsl:if test="@prior">
				<xsl:attribute name="prior">
					<xsl:value-of select="@prior + $num"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="@*|node()|text()"/>
		</xsl:copy>
	</xsl:template>

	<!-- Usuwamy poprzednia wartosc prior -->
	<xsl:template match="ipk:param//ipk:draw[contains(@title,$title)]/@prior"/>

	<!-- Kopiujemy reszt� -->
	<xsl:template match="@*|node()|text()|comment()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()|text()|comment()"/>
		</xsl:copy>
	</xsl:template>

</xsl:stylesheet>

