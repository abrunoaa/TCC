MAIN=main-tcc

all: $(MAIN).tex
	pdflatex --file-line-error-style $(MAIN) \
	&& bibtex $(MAIN) \
	&& pdflatex --file-line-error-style $(MAIN) \
	&& pdflatex --file-line-error-style $(MAIN)

clean:
	find . -type f \( -name '*.aux' -o -name '*.bbl' -o -name '*.blg' -o -name '*.idx' -o -name '*.log' -o -name '*.pdf' \) -delete
