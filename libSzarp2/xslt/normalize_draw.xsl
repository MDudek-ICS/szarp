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

	$Id: normalize_draw.xsl 4393 2007-09-13 16:37:10Z reksio $

	Ten szablon normalizuje wszystkie atrybuty 'prior' element�w 'draw' do
	postaci '1.X', albo przez wzi�cie za X cz�ci u�amkowej poprzedniej
	warto�ci atrybutu, albo ca�o�ci. Przyk�adowo 4.56 zostanie zamienione
	na 1.56, a 87 na 1.87.

 -->
 
<xsl:stylesheet version="1.0" 
	xmlns:ipk="http://www.praterm.com.pl/SZARP/ipk"
	xmlns="http://www.praterm.com.pl/SZARP/ipk"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	
	<!-- Priorytety -->
	<xsl:template match="ipk:param//ipk:draw[@prior]">
		<xsl:copy>
			<xsl:attribute name="prior">
				<xsl:choose>
					<xsl:when test="contains(@prior,'.')">
						<xsl:value-of 
						select="concat('1.', 
							substring-after(@prior,'.'))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of 
						select="concat('1.', @prior)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:apply-templates select="@*|node()|text()"/>
		</xsl:copy>
	</xsl:template>

	<!-- Usuwamy poprzednia wartosc prior -->
	<xsl:template match="ipk:param//ipk:draw/@prior"/>

	<!-- Kopiujemy reszt� -->
	<xsl:template match="@*|node()|text()|comment()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()|text()|comment()"/>
		</xsl:copy>
	</xsl:template>

</xsl:stylesheet>

