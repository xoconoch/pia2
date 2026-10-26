#include "paclib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PACSTR                                                                 \
  "{id:%d|tipo:%d|nombre:%s|edad:%d|genero:%d|sintomas:%s|consultorio:%d|"     \
  "esMenor:%d|menor.edad:%d|menor.nombre:%s}\n"

#define PACSEARCH                                                              \
  "{id:%d|tipo:%d|nombre:%99[^|]|edad:%d|genero:%d|sintomas:%199[^|]|"         \
  "consultorio:%d|esMenor:%d|menor.edad:%d|menor.nombre:%99[^}]}\n"

int altaPaciente(FILE *ar, Paciente *p) {
  // Pasar archivo en modo `a`
  if (ar == NULL || p == NULL)
    return 1;

  if (p->nombre[0] == '\0')
    strcpy(p->nombre, "NA");

  if (p->sintomas[0] == '\0')
    strcpy(p->sintomas, "NA");

  if (p->menor.nombre[0] == '\0')
    strcpy(p->menor.nombre, "NA");

  fprintf(ar, PACSTR, p->id, p->tipo, p->nombre, p->edad, p->genero,
          p->sintomas, p->consultorio, p->esMenor, p->menor.edad,
          p->menor.nombre);
  return 0;
}

int tomarPacienteID(FILE *ar, Paciente **p, int id) {
  // Pasar archivo en modo `r`
  if (ar == NULL || p == NULL || *p == NULL)
    return 1;

  rewind(ar);

  while (fscanf(ar, PACSEARCH, &(*p)->id, &(*p)->tipo, (*p)->nombre,
                &(*p)->edad, &(*p)->genero, (*p)->sintomas, &(*p)->consultorio,
                &(*p)->esMenor, &(*p)->menor.edad, (*p)->menor.nombre) == 10) {
    if ((*p)->id == id)
      return 0;
  }

  *p = NULL;
  return 1;
}

int tomarPacienteNom(FILE *ar, Paciente **p, char nombre[100]) {
  // Pasar archivo en modo `r`
  if (ar == NULL || p == NULL)
    return 1;

  *p = NULL;
  int cont = 0;
  Paciente temp;

  rewind(ar);

  while (fscanf(ar, PACSEARCH, &temp.id, &temp.tipo, temp.nombre, &temp.edad,
                &temp.genero, temp.sintomas, &temp.consultorio, &temp.esMenor,
                &temp.menor.edad, temp.menor.nombre) == 10) {

    if (strcmp(temp.nombre, nombre) == 0) {
      Paciente *aux = realloc(*p, (cont + 1) * sizeof(Paciente));

      if (aux == NULL) {
        free(*p);
        *p = NULL;
        return 1;
      }

      *p = aux;
      (*p)[cont] = temp;
      cont++;
    }
  }

  return cont;
}

int editarPaciente(FILE *ar, int id, Paciente edit) {
  // Pasar archivo en modo `r+`
  if (ar == NULL)
    return 1;

  FILE *tmp = tmpfile();
  if (tmp == NULL)
    return 1;

  Paciente p;
  int encontrado = 0;

  rewind(ar);

  // Este ciclo copia todo `ar` a `tmp` pero con el paciente ya modificado
  while (fscanf(ar, PACSEARCH, &p.id, &p.tipo, p.nombre, &p.edad, &p.genero,
                p.sintomas, &p.consultorio, &p.esMenor, &p.menor.edad,
                p.menor.nombre) == 10) {
    if (p.id == id) {
      p = edit;
      encontrado = 1;
    }

    fprintf(tmp, PACSTR, p.id, p.tipo, p.nombre, p.edad, p.genero, p.sintomas,
            p.consultorio, p.esMenor, p.menor.edad, p.menor.nombre);
  }

  // Verificamos que se haya editado al menos un paciente
  if (!encontrado) {
    fclose(tmp);
    return 1;
  }

  // Copiamos `tmp` a `ar`
  rewind(tmp);
  rewind(ar);

  int c;
  while ((c = fgetc(tmp)) != EOF)
    fputc(c, ar);

  // Asegura que todo se haya escrito ya
  fflush(ar);

  // Es posible que `tmp` sea más pequeño que `ar`, por lo que truncamos los
  // bytes sobrantes después de la escritura
  ftruncate(fileno(ar), ftell(ar));

  fclose(tmp);
  return 0;
}

int removerPaciente(FILE *ar, int id, Paciente edit) {
  // Pasar archivo en modo `r+`
  if (ar == NULL)
    return 1;

  FILE *tmp = tmpfile();
  if (tmp == NULL)
    return 1;

  Paciente p;
  int encontrado = 0;

  rewind(ar);

  // Este ciclo copia todo `ar` a `tmp` pero con el paciente ya modificado
  while (fscanf(ar, PACSEARCH, &p.id, &p.tipo, p.nombre, &p.edad, &p.genero,
                p.sintomas, &p.consultorio, &p.esMenor, &p.menor.edad,
                p.menor.nombre) == 10) {
    if (p.id != id) {
      fprintf(tmp, PACSTR, p.id, p.tipo, p.nombre, p.edad, p.genero, p.sintomas,
              p.consultorio, p.esMenor, p.menor.edad, p.menor.nombre);
    } else {
      encontrado = 1;
    }
  }

  // Verificamos que se haya editado al menos un paciente
  if (!encontrado) {
    fclose(tmp);
    return 1;
  }

  // Copiamos `tmp` a `ar`
  rewind(tmp);
  rewind(ar);

  int c;
  while ((c = fgetc(tmp)) != EOF)
    fputc(c, ar);

  // Asegura que todo se haya escrito ya
  fflush(ar);

  // Es posible que `tmp` sea más pequeño que `ar`, por lo que truncamos los
  // bytes sobrantes después de la escritura
  ftruncate(fileno(ar), ftell(ar));

  fclose(tmp);
  return 0;
}
