\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{abntex2}[2018/11/24 v-1.9.7 abnTeX2]
\RequirePackage{ifthen}

% repassa os parâmetros ao abntex2
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{abntex2}}

\PassOptionsToClass{%
    12pt,                     % tamanho da fonte
    a4paper,                  % tamanho do papel.
    sumario=tradicional,      % seleciona o sumário tradicional
    brazil,                   % idioma do documento
}{abntex2}

\ProcessOptions\relax
\LoadClass{abntex2}

\RequirePackage[brazilian, hyperpageref]{} % produzir índice nas referências
%\RequirePackage[brazilian, hyperpageref]{backref} % produzir índice nas referências
\RequirePackage{makeidx}                          % produzir índice remissivo de palavras
\RequirePackage{chngcntr}

% -----------------------------------------------------------------------------
% configurações gerais
% -----------------------------------------------------------------------------

% retira o espaço extra desnecessário nas frases
\frenchspacing

% configurações do pacote backref usado sem a opção hyperpageref de backref
%\renewcommand{\backrefpagesname}{Citado na(s) p\'agina(s):~}

% Texto padrão antes do número das páginas
%\renewcommand{\backref}{}

% define as listas de item compactas (sem espaçamento entre os itens)
\setlist{noitemsep, topsep=0pt, parsep=0pt, partopsep=0pt}

% define os textos da citação
%\renewcommand*{\backrefalt}[4]
%{%
%    \ifcase #1
%        Nenhuma cita\c{c}\~ao no texto.
%    \or
%        Citado na p\'agina #2.
%    \else
%        Citado #1 vezes nas p\'aginas #2.
%    \fi
%}

% define a numeração das equações de forma sequencial
\counterwithout{equation}{chapter}

% -----------------------------------------------------------------------------
% estilo de elementos
% -----------------------------------------------------------------------------

% elementos pré-textuais
\renewcommand{\pretextual}
{%
    \pagenumbering{gobble}
    \aliaspagestyle{chapter}{empty}
    \pagestyle{empty}
    \aliaspagestyle{cleared}{empty}
    \aliaspagestyle{part}{empty}
}

% configura layout para elementos textuais
\renewcommand{\textual}
{%
    \pagenumbering{arabic} % A numeração das páginas é em algarismos arábicos, inciando em 1
    \pagestyle{abntheadings}
    \aliaspagestyle{chapter}{abntchapfirst}
    \nouppercaseheads
    \bookmarksetup{startatroot}
}

% configura layout para elementos pós-textuais
\renewcommand{\postextual}
{%
    \chapterstyle{abnt}
    \phantompart
}

% -----------------------------------------------------------------------------
% objetos flutuantes
% -----------------------------------------------------------------------------

% comando que trata a fonte nos objetos flutuantes
\renewcommand{\fonte}[1]
{%
    \begin{SingleSpacing}\par\end{SingleSpacing}
    \centering\small{Fonte: #1}
}

% comando que trata as notas nos objetos flutuantes
\renewcommand{\nota}[1]
{%
    \begin{SingleSpacing}\par\end{SingleSpacing}
    \begin{tabular}{l p{.85\textwidth}}%
        Nota: & #1
    \end{tabular}
}

% -----------------------------------------------------------------------------
% parágrafos
% -----------------------------------------------------------------------------

% tamanho da indentação do parágrafo
\setlength{\parindent}{1.5cm}

% espaçamento entre um parágrafo e outro
\setlength{\parskip}{0.0cm}

% -----------------------------------------------------------------------------
% fontes
% -----------------------------------------------------------------------------

% redefinição de fontes
\renewcommand{\ABNTEXchapterfont}{\normalfont\bfseries}
\renewcommand{\ABNTEXchapterfontsize}{\normalsize}

\renewcommand{\ABNTEXpartfont}{\normalfont}
\renewcommand{\ABNTEXpartfontsize}{\ABNTEXchapterfontsize}

\renewcommand{\ABNTEXsectionfont}{\normalfont\bfseries}
\renewcommand{\ABNTEXsectionfontsize}{\normalsize}

\renewcommand{\ABNTEXsubsectionfont}{\normalfont\bfseries}
\renewcommand{\ABNTEXsubsectionfontsize}{\normalsize}

\renewcommand{\ABNTEXsubsubsectionfont}{\normalfont\bfseries}
\renewcommand{\ABNTEXsubsubsectionfontsize}{\normalsize}

\renewcommand{\ABNTEXsubsubsubsectionfont}{\normalfont\bfseries}
\renewcommand{\ABNTEXsubsubsubsectionfontsize}{\normalsize}

% -----------------------------------------------------------------------------
% sumário
% -----------------------------------------------------------------------------

% travessão no sumário para apêndices e anexos
%\renewcommand{\cftchapteraftersnum}{\hfill\textendash\hfill}

% insere espaço entre os itens do sumário de diferentes capítulos
\setlength{\cftbeforechapterskip}{1.0em \@plus\p@}

% -----------------------------------------------------------------------------
% listas
% -----------------------------------------------------------------------------

% define os títulos das listas
\addto\captionsbrazil{%
    % ajusta nomes padrões do babel
    \renewcommand{\listadesiglasname}{LISTA DE ABREVIATURAS E SIGLAS}
    \renewcommand{\listadesimbolosname}{LISTA DE S\'{I}MBOLOS}
    \renewcommand{\listfigurename}{LISTA DE FIGURAS}
    \renewcommand{\listtablename}{LISTA DE TABELAS}
    \renewcommand{\indexname}{\'{I}NDICE REMISSIVO}
    % ajusta nomes usados com a macro \autoref
    \renewcommand{\pageautorefname}{p\'agina}
    \renewcommand{\sectionautorefname}{Se\c{c}\~ao}
    \renewcommand{\subsectionautorefname}{Subse\c{c}\~ao}
    \renewcommand{\subsubsectionautorefname}{Subsubse\c{c}\~ao}
    \renewcommand{\paragraphautorefname}{Subse\c{c}\~ao}
}

% -----------------------------------------------------------------------------
% dedicatória
% -----------------------------------------------------------------------------
\renewenvironment{dedicatoria}[1][]
{%
    \ifthenelse{\equal{#1}{}}{%
        \PRIVATEbookmarkthis{\dedicatorianame}
    }{\pretextualchapter{#1}}

    \vspace*{\fill}

    \begin{flushright}
        \begin{minipage}{.5\textwidth}
            \begin{SingleSpacing}
                \setlength\parindent{0pt}
                \setlength\parskip{12pt}
}
{%
            \end{SingleSpacing}
        \end{minipage}
    \end{flushright}
}

% -----------------------------------------------------------------------------
% epígrafe
% -----------------------------------------------------------------------------
\renewenvironment{epigrafe}[1][]
{%
    \ifthenelse{\equal{#1}{}}
    {%
        \PRIVATEbookmarkthis{\epigraphname}
    }{\pretextualchapter{#1}}

    \vspace*{\fill}

    \begin{flushright}
        \begin{minipage}{.5\textwidth}
            \begin{SingleSpacing}
                \setlength\parindent{0pt}
                \setlength\parskip{12pt}
}
{%
            \end{SingleSpacing}
        \end{minipage}
    \end{flushright}
}
