" Vim syntax file
" Language:     SZARP config file
" Maintainer:   Pawel Palucha <pawel@praterm.com.pl>
" Last_change:  6 Nov 2015
"
" To jest plik definicji dla pod�wietlania sk�adni plik�w konfiguracyjnych
" SZARPA w edytorze Vim. Dzia�a, cho� miejscami jest bardziej restrykcyjny
" ni� parser libpara.
" Aby za�adowa� r�cznie pod�wietlanie sk�adni SZARPA nale�y w Vim'ie wykona� 
" nast�puj�ce komendy:
"   :au! Syntax szarp so libpar.vim
"   :set syntax=szarp

if exists("b:current_syntax")
    finish
endif

syn clear

if ("version" >= 610)
    syn sync fromstart
else
    syn sync minlines=1000
endif

syn match ddolar "\$\$"
" Wszystko co nie jest dobre, jest z�e
syn match unknownCharacter "^.*$"
" Bia�e spacje s� dobre
syn match ok "^[ \t]*$"
" Komentarze od pocz�tku linii do ko�ca
syn region szarpComment  start="^#"  end="$" contains=commentSpecial
syn match commentSpecial contained "\\$"
" Operatory
syn match szarpOp "<>\|=\|:="
" Nazwa parametru - od pocz�tku linii do znaku "="
syn match paramName "^[a-zA-Z_\$][a-zA-Z0-9_\$]*="me=e-1 contains=var,ddolar
"syn match paramName "^[a-zA-Z_\$][a-zA-Z0-9_\$]*=" contains=var,ddolar

" Zawarto�� parametru - od r�wna si� do konca linii
syn region paramContent start="=" end="$" contains=paramSpecial,var,ddolar
syn match paramSpecial contained "\\$"
" Sekcje
syn match szarpSection "^:[a-zA-Z_\$][a-zA-Z0-9_\$]*$" contains=var,ddolar

" Formu�y
syn region szarpFormula start="^\$" end="$" contains=commentSpecial,szarpKeyword,szarpFunction,szarpParams,szarpString,szarpOp,whitespaces,var,ddolar
" Wywo�ania funkcji - to nie do ko�ca dzia�a, bo mi�dzy nazw� funkcji
" a nawiasem nie mo�e by� spacji (parser na to pozwala) - ale nie wiem
" jak to zrobi�... Jak kto� wie, niech poprawi ;-)
syn match szarpFunction "\<[a-zA-Z_\$][a-zA-Z0-9_\$]*[:space:]*\>("me=e-1 contains=var,ddolar
" Nawiaski
syn region szarpParams start="(" end=")" contains=szarpString,szarpFunction,szarpParams,var,ddolar
syn match whitespaces contained "[ \t]"
" Stringi
syn region szarpString start=+"+ skip=+\\"+ end=+"+ contains=stringSpecial,var,ddolar
syn match stringSpecial contained "\\[nt\\\"]\|\\$\|\$\$"
" S�owa kluczowe
syn match szarpKeyword contained "\$if\|\$elseif\|\$else\|\$end"
" Dyrektywa $include
syn match d_include "^\$include .*" contains=szarpString

syn match var "\$[a-zA-Z_][a-zA-Z0-9_]*\$"

syn match blueones "[a-zA-Z_0-9]+"

if !exists("did_szarp_syntax_inits")
    let did_szarp_syntax_inits = 1
    hi link d_include        Special
    hi link szarpKeyword     Special
    hi link szarpComment     Comment
    hi link adaCharacter     Character
    hi link paramName        PreProc
    hi link blueones         PreProc
    hi link paramContent     String
    hi link commentSpecial   Special
    hi link paramSpecial     Special
    hi link szarpFormula     Error
    hi link dolar            Special
    hi link szarpFunction    PreProc
    hi link szarpString      String
    hi link szarpParams      Keyword
    hi link stringSpecial    Special
    hi link ddolar           Special
    hi link szarpOp          Operator
    hi link unknownCharacter Error
    hi link szarpSection     Label
    hi link var              Type
endif

let b:current_syntax = "szarp"

