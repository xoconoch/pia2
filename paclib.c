#include "paclib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PACSTR                                                                 \
  "{id:%d|tipo:%d|nombre:%s|direccion:%s|edad:%d|genero:%d|sintomas:%s|"       \
  "consultorio:%d|"                                                            \
  "esMenor:%d|menor.edad:%d|menor.nombre:%s}\n"

#define PACSEARCH                                                              \
  "{id:%d|tipo:%d|nombre:%99[^|]|direccion:%199[^|]|edad:%d|genero:%d|"        \
  "sintomas:%199[^|]|"                                                         \
  "consultorio:%d|esMenor:%d|menor.edad:%d|menor.nombre:%99[^}]}\n"

int altaPaciente(FILE *ar, Paciente *p) {
  // Pasar archivo en modo `a+`
  if (ar == NULL || p == NULL)
    return 1;

  Paciente temp;

  if (p->nombre[0] == '\0')
    strcpy(p->nombre, "NA");

  if (p->direccion[0] == '\0')
    strcpy(p->direccion, "NA");

  if (p->sintomas[0] == '\0')
    strcpy(p->sintomas, "NA");

  if (p->menor.nombre[0] == '\0')
    strcpy(p->menor.nombre, "NA");

  // Aseguramos que empieza a leer desde el principio del archivo
  rewind(ar);

  while (fscanf(ar, PACSEARCH, &temp.id, &temp.tipo, temp.nombre,
                temp.direccion, &temp.edad, &temp.genero, temp.sintomas,
                &temp.consultorio, &temp.esMenor, &temp.menor.edad,
                temp.menor.nombre) == 11) {
    if (temp.id == p->id)
      return 3;
    if (temp.consultorio == p->consultorio)
      return 2;
  }

  // Desplazar hasta el final del archivo
  fseek(ar, 0, SEEK_END);
  fprintf(ar, PACSTR, p->id, p->tipo, p->nombre, p->direccion, p->edad,
          p->genero, p->sintomas, p->consultorio, p->esMenor, p->menor.edad,
          p->menor.nombre);
  // Limpiar posible basura en el buffer
  fflush(ar);
  return 0;
}

int tomarPacienteID(FILE *ar, Paciente *p, int id) {
  // Pasar archivo en modo `r`
  if (ar == NULL || p == NULL)
    return 1;

  rewind(ar);

  Paciente temp;
  // lee los parametros de cada paciente
  // guarda el paciente que se busca en el puntero solo si se encuentra
  while (fscanf(ar, PACSEARCH, &temp.id, &temp.tipo, temp.nombre,
                temp.direccion, &temp.edad, &temp.genero, temp.sintomas,
                &temp.consultorio, &temp.esMenor, &temp.menor.edad,
                temp.menor.nombre) == 11) {
    if (temp.id == id) {
      // Primero guardamos la data en `temp` para no contaminar el puntero `p`
      // con basura en caso de que no se encuentre ningún paciente
      *p = temp;
      return 0;
    }
  }

  return 1;
}

int tomarPacienteNom(FILE *ar, Paciente **p, char nombre[100]) {
  // Pasar archivo en modo `r`
  if (ar == NULL || p == NULL)
    return 0;

  *p = NULL;
  int cont = 0;
  Paciente temp;

  rewind(ar);

  while (fscanf(ar, PACSEARCH, &temp.id, &temp.tipo, temp.nombre,
                temp.direccion, &temp.edad, &temp.genero, temp.sintomas,
                &temp.consultorio, &temp.esMenor, &temp.menor.edad,
                temp.menor.nombre) == 11) {

    if (strcmp(temp.nombre, nombre) == 0 ||
        (temp.esMenor && strcmp(temp.menor.nombre, nombre) == 0)) {
      // al puntero se le asigna un espacio mas a la memoria
      Paciente *aux = realloc(*p, (cont + 1) * sizeof(Paciente));

      if (aux == NULL) {
        free(*p);
        *p = NULL;
        return 0;
      }

      *p = aux;
      // el contenido de temp se asigna al ultimo elemento del arreglo
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

  // Se aplica la misma sanitización que en `altaPaciente`
  if (edit.nombre[0] == '\0')
    strcpy(edit.nombre, "NA");
  if (edit.direccion[0] == '\0')
    strcpy(edit.direccion, "NA");
  if (edit.sintomas[0] == '\0')
    strcpy(edit.sintomas, "NA");
  if (edit.menor.nombre[0] == '\0')
    strcpy(edit.menor.nombre, "NA");

  rewind(ar);
  while (fscanf(ar, PACSEARCH, &p.id, &p.tipo, p.nombre, p.direccion, &p.edad,
                &p.genero, p.sintomas, &p.consultorio, &p.esMenor,
                &p.menor.edad, p.menor.nombre) == 11) {
    if (p.consultorio == edit.consultorio && p.id != id) {
      fclose(tmp);
      return 2;
    }
  }

  rewind(ar);

  // Este ciclo copia todo `ar` a `tmp` pero con el paciente ya modificado
  while (fscanf(ar, PACSEARCH, &p.id, &p.tipo, p.nombre, p.direccion, &p.edad,
                &p.genero, p.sintomas, &p.consultorio, &p.esMenor,
                &p.menor.edad, p.menor.nombre) == 11) {
    if (p.id == id) {
      p = edit;
      encontrado = 1;
    }

    fprintf(tmp, PACSTR, p.id, p.tipo, p.nombre, p.direccion, p.edad, p.genero,
            p.sintomas, p.consultorio, p.esMenor, p.menor.edad, p.menor.nombre);
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

int removerPaciente(FILE *ar, int id) {
  // Pasar archivo en modo `r+`
  if (ar == NULL)
    return 1;

  FILE *tmp = tmpfile();
  if (tmp == NULL)
    return 1;

  Paciente p;
  int encontrado = 0;

  rewind(ar);

  // Este ciclo copia todo `ar` a `tmp` pero con el paciente ya removido
  while (fscanf(ar, PACSEARCH, &p.id, &p.tipo, p.nombre, p.direccion, &p.edad,
                &p.genero, p.sintomas, &p.consultorio, &p.esMenor,
                &p.menor.edad, p.menor.nombre) == 11) {
    if (p.id != id) {
      fprintf(tmp, PACSTR, p.id, p.tipo, p.nombre, p.direccion, p.edad,
              p.genero, p.sintomas, p.consultorio, p.esMenor, p.menor.edad,
              p.menor.nombre);
    } else {
      encontrado = 1;
    }
  }

  // Verificamos que se haya removido al menos un paciente
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

int listarPacientes(FILE *ar) {
  if (ar == NULL)
    return 1;

  Paciente temp;

  rewind(ar);

  printf("+--------+----+------------------------------+-----------------------"
         "-------+----+---------+------------------------------+------------+--"
         "----------+--------------+\n");
  printf("|%-8s|%-4s|%-30s|%-30s|%-4s|%-9s|%-30s|%-12s|%-12s|%-14s|\n", "Id",
         "Tipo", "Nombre (Tutor/Paciente)", "Direccion", "Edad", "Genero",
         "Sintomas", "Consultorio", "Edad (Menor)", "Nombre (Menor)");
  printf("+--------+----+------------------------------+-----------------------"
         "-------+----+---------+------------------------------+------------+--"
         "----------+--------------+\n");

  while (fscanf(ar, PACSEARCH, &temp.id, &temp.tipo, temp.nombre,
                temp.direccion, &temp.edad, &temp.genero, temp.sintomas,
                &temp.consultorio, &temp.esMenor, &temp.menor.edad,
                temp.menor.nombre) == 11) {

    printf("|%-8d|%-4d|%-30s|%-30s|%-4d|%-9d|%-30s|%-12d|%-12d|%-14s|\n",
           temp.id, temp.tipo, temp.nombre, temp.direccion, temp.edad,
           temp.genero, temp.sintomas, temp.consultorio, temp.menor.edad,
           temp.menor.nombre);
  }
  printf("+--------+----+------------------------------+-----------------------"
         "-------+----+---------+------------------------------+------------+--"
         "----------+--------------+\n");
  return 0;
}

void menu() {
  printf("|--Hospital Bisonte--|\n\n");
  printf("1. Alta de paciente\n2. Busqueda o Edicion de Paciente.\n");
  printf("3. Listado de Pacientes\n4. Baja de Paciente.\n");
  printf("5. Salir\n");
  printf("Ingrese una opcion: ");
}

void subMenu() {
  printf("(a) Busqueda por ID.\n");
  printf("(b) Busqueda por Nombre.\n");
  printf("(c) Editar Paciente.\n");
  printf("Ingrese una opcion: ");
}
