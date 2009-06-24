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

	$Id: select_with_name.xsl 4393 2007-09-13 16:37:10Z reksio $

	Ten szablon wybiera z dokumentu tylko te parametry, kt�re zawieraj�
	podany ci�g znak�w w atrybucie 'name'. Ci�g podaje si� jako parametr 
	'search', np. dla procesora xsltproc wywo�anie powinno mie� posta�:
		xsltproc ++stringparam select_with_name.xsl document.xml
	(plusy nale�y zamieni� ma minusy, tylko vim si� na tym komentarzy
	wywala).
 -->
 
<xsl:stylesheet version="1.0" 
	xmlns:ipk="http://www.praterm.com.pl/SZARP/ipk"
	xmlns="http://www.praterm.com.pl/SZARP/ipk"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<!-- Deklaracja parametru -->
	<xsl:param name="search"/>
	 
	<!-- Kopiujemy wszystkie elementy 'param' zawieraj�ce szukany ci�g. -->
	<xsl:template match="ipk:param[contains(@name,$search)]">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()|comment()"/>
		</xsl:copy>
	</xsl:template>

	<!-- Ignorujemy wszystkie inne elementy 'param' -->
	<xsl:template match="ipk:param">
	</xsl:template>

	<!-- Tu probujemy troch� walczy� z pustymi liniami -->
	<xsl:template match="text()[position()=last()]">
		<xsl:copy/>
	</xsl:template>

	<xsl:template match="text()[position()>1]">
	</xsl:template>

	<!-- Kopiujemy reszt� bez zmian -->
  	<xsl:template match="@*|node()|comment()">
      		<xsl:copy>
            		<xsl:apply-templates select="@*|node()"/>
	        </xsl:copy>
	</xsl:template>
	
</xsl:stylesheet>
