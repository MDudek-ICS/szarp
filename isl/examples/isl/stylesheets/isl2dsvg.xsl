<?xml version="1.0"?>
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
	(C) Pawel Palucha 2002

	$Id: isl2dsvg.xsl 4393 2007-09-13 16:37:10Z reksio $

	This stylesheet transforms isl document to dynamic, scripted
	SVG document. It needs a browser with dynamic DOM modifications
	capabilty (adding and removing elements). Currently, only Batik version
	1.5 (beta) is known to work.

	All elements with attributes "isl:uri" are replaced by proper "<use>" 
	elements, with "xlink:href" attributes pointing to elements defined
	in document generated by isl2defs.xsl stylesheet. Scripts in ECMA
	script are added to reload this attributes every 10 seconds.

	Other elements are copied without modifications.

	Because of bugs in current version of Batik, every "dynamic" element
	must be inserted to <g> element.
	
 -->

<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:isl="http://www.praterm.com.pl/ISL/params"
	xmlns:xlink="http://www.w3.org/1999/xlink"
	xmlns="http://www.w3.org/2000/svg"
	xmlns:svg="http://www.w3.org/2000/svg">

	<!-- We need to know the name of processed document, to create link to
	definitions document. -->
	<xsl:param name="docpath"/>
	<xsl:param name="docname"/>

	<!-- Add scripts to main element. -->
	<xsl:template match="svg:svg">
		<svg>
			<xsl:for-each select="@*">
				<xsl:copy/>
			</xsl:for-each>
			<xsl:attribute
				name="onload">start_isl(evt)</xsl:attribute>

	<script type="text/ecmascript"><![CDATA[
		var svgNamespaceURI = "http://www.w3.org/2000/svg";
		var xlinkNamespaceURI = "http://www.w3.org/1999/xlink";
	    
		var objs = new Array ();
		var reload = 0;
		var rrate = 10000;
	    
		function start_isl(evt) {
			setTimeout("anim_isl()", rrate);
		}
		
		function add_isl(evt) {
			var l = objs.length;
			
			objs[l] = evt.target;
		}
		
		function anim_isl() {
			var j, c, c2;	// counters
			var e, p, e2;
			var u, u2;

			reload++;
			if (reload &gt; 1000)
				reload = 1;
				for (j = 0; j &lt; objs.length; j++) {
		    		e = objs[j];
		    		p = e.parentNode;
				u = e.getAttributeNS(xlinkNamespaceURI, "href");
				p.removeChild(e);
				c = u.lastIndexOf("/");
				c2 = u.indexOf("defs");
				if (c &gt;= 0 &amp;&amp; c2 &gt;= 0) {
					u2 = u.substring(c+1, u.length());
					u = u.substring(0, c2);
					u = u.concat("defs/reload/" + reload + "/" + u2);
				}
				e2 = create_element(u);
				p.appendChild(e2);
				
				objs[j] = e2;
			}
			setTimeout("anim_isl()", rrate);
		}

		function create_element(uri) {
		
	    		var e = document.createElementNS
					(svgNamespaceURI, "svg:use");
        		e.setAttributeNS(xlinkNamespaceURI, "xlink:href", uri);
        		return e;
    		}
			
		window.start_isl = start_isl;
		window.add_isl = add_isl;
		window.anim_isl = anim_isl;
		window.create_element = create_element;
    ]]></script>
	
			<xsl:apply-templates />
		</svg>
	</xsl:template>
	
	<!-- Replace isl elements with "use" elements. -->
	<xsl:template match="*[@isl:uri]">
		<g><use>
			<xsl:variable name="curr" select="."/>
			<xsl:variable name="gen_id" select="@id"/>
			<xsl:attribute name="xlink:href"> 
				<xsl:value-of select="concat(
					'/',
					$docpath,
					'/defs/',
					$docname,'.svg#',$gen_id)"/>
			</xsl:attribute>
			<xsl:attribute
				name="onload">add_isl(evt)</xsl:attribute>
		</use></g>
		
	</xsl:template> 
	
	<!-- Copy other elements -->
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template> 
  
</xsl:stylesheet>
