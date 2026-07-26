# TensorForth

Interprete scritto in C per operazioni su tensori, basato su uno stack e notazione postfissa (in stile Forth).

## Descrizione

TensorForth è un interprete stack-based che permette di eseguire operazioni su tensori usando la notazione postfissa. Il progetto implementa un motore di calcolo efficiente con parallelizzazione OpenMP.

## Caratteristiche principali

- **Notazione postfissa**: tutte le operazioni vengono espresse in stile Forth, con gli operandi che precedono l'operatore
- **Set completo di operatori** per la manipolazione di tensori
- **Parallelizzazione OpenMP**: ottimizzazione delle operazioni su più dimensioni
- **I/O binario efficiente**: lettura/scrittura tramite `mmap` con allineamento a 64 byte
- **Gestione della memoria**: sistema di reference counting per evitare leak e copie inutili

## Requisiti

- Compilatore C compatibile con OpenMP (es. `gcc`)
- Sistema Linux (il Makefile è configurato per questo ambiente)
- Libreria OpenMP installata

## Compilazione

```bash
make
```

## Utilizzo

```bash
./tensorforth <file.tensorforth>
```

## Scopo Progetto

Progetto sviluppato per il corso "Programmazione Avanazata e Parallela" insieme a un secondo progetto Python.
