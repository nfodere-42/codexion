#!/bin/bash

BIN=./codexion
TIMEOUT=3

# Colores
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"

ok()   { echo -e "${GREEN}[OK]${RESET} $1"; }
fail() { echo -e "${RED}[KO]${RESET} $1"; exit 1; }

run_test() {
    DESC="$1"
    shift
    PARAMS="$@"

    echo -e "${YELLOW}==> $DESC${RESET}"
    OUTPUT=$(timeout $TIMEOUT $BIN $PARAMS 2>&1)
    RET=$?

    # Timeout
    if [ $RET -eq 124 ]; then
        fail "El programa se ha quedado colgado con parámetros: $PARAMS"
    fi

    # Crash
    if [ $RET -ne 0 ]; then
        fail "El programa ha devuelto código de error con parámetros: $PARAMS"
    fi

    # Log vacío
    if [ -z "$OUTPUT" ]; then
        fail "Salida vacía con parámetros: $PARAMS"
    fi

    # Burnout después de logs posteriores
    if echo "$OUTPUT" | grep -q "burned out"; then
        LAST_LINE=$(echo "$OUTPUT" | tail -n 1)
        if ! echo "$LAST_LINE" | grep -q "burned out"; then
            fail "Hay logs después del burnout con parámetros: $PARAMS"
        fi
    fi

    # Formato incorrecto
    while read -r line; do
        if ! echo "$line" | grep -Eq "^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$"; then
            fail "Formato de log incorrecto: '$line' con parámetros: $PARAMS"
        fi
    done <<< "$OUTPUT"

    ok "$DESC"
}

#############################################
# TESTS
#############################################

run_test "Test básico FIFO" \
    2 500 100 100 100 2 50 fifo

run_test "Test básico EDF" \
    2 500 100 100 100 2 50 edf

run_test "Test con 3 coders FIFO" \
    3 600 120 80 90 2 30 fifo

run_test "Test con 3 coders EDF" \
    3 600 120 80 90 2 30 edf

run_test "Test cooldown alto" \
    3 500 100 100 100 2 300 fifo

run_test "Test burnout inevitable" \
    2 200 300 100 100 1 0 fifo

run_test "Test de estrés 5 coders" \
    5 800 100 100 100 3 50 fifo

run_test "Test de estrés 5 coders EDF" \
    5 800 100 100 100 3 50 edf

echo -e "${GREEN}TODAS LAS PRUEBAS PASARON CORRECTAMENTE${RESET}"

