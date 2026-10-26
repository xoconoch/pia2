#ifndef PACLIB_H_INCLUDED
#define PACLIB_H_INCLUDED

#include <stdio.h>

typedef struct {
  int edad;
  char nombre[100];
} Menor;

typedef struct {
  int id;
  int tipo; // 0 = externa; 1 = emergencia;
  char nombre[100];
  int edad;
  int genero;
  char sintomas[200];
  int consultorio;

  int esMenor; // 0 = si; 1 = no;
  Menor menor;
} Paciente;

int altaPaciente(FILE *ar, Paciente *p);
int tomarPacienteID(FILE *ar, Paciente **p, int id);
int tomarPacienteNom(FILE *ar, Paciente **p, char nombre[100]);
int editarPaciente(FILE *ar, int id, Paciente edit);

#endif // !PACLIB_H_INCLUDED
