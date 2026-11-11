#include "paclib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  FILE *ar;
  Paciente p;
  char nombre[100];
  int sentinela = 0, i, op, op2, id;

  do {
    menu();
    scanf("%d", &op);

    do {
      printf("Ingrese una opcion:");
      scanf("%d", &op);
    } while (op < 1 || op > 5);

    switch (op) {

    case 1:

      printf("Ingrese los datos del paciente:\n");
      printf("ID: ");
      scanf("%d", &p.id);

      do {
        printf("Tipo de Consulta (0=Externa, 1=Emergencia): ");
        scanf("%d", &p.tipo);
      } while (p.tipo < 0 || p.tipo > 1);

      while (getchar() != '\n')
        ;

      printf("Nombre: ");
      fgets(p.nombre, sizeof(p.nombre), stdin);
      p.nombre[strcspn(p.nombre, "\n")] = '\0';

      printf("Direccion: ");
      fgets(p.direccion, sizeof(p.direccion), stdin);
      p.direccion[strcspn(p.direccion, "\n")] = '\0';

      do {
        printf("Edad (Mayor de Edad): ");
        scanf("%d", &p.edad);
      } while (p.edad < 18 || p.edad > 130);

      do {
        printf("Genero (0=Masculino, 1=Femenino, 2=No Binario): ");
        scanf("%d", &p.genero);
      } while (p.genero < 0 || p.genero > 2);

      while (getchar() != '\n')
        ;

      printf("Sintomas: ");
      fgets(p.sintomas, sizeof(p.sintomas), stdin);
      p.sintomas[strcspn(p.sintomas, "\n")] =
          '\0'; // Eliminar el salto de línea al final

      if (p.tipo == 0) {
        do {
          printf("Consultorio (1-5): ");
          scanf("%d", &p.consultorio);
        } while (p.consultorio < 1 || p.consultorio > 5);
      } else {
        do {
          printf("Consultorio (6-10): ");
          scanf("%d", &p.consultorio);
        } while (p.consultorio < 6 || p.consultorio > 10);
      }

      do {
        printf("Es menor de edad (1=si, 0=no): ");
        scanf("%d", &p.esMenor);
      } while (p.esMenor < 0 || p.esMenor > 1);

      if (p.esMenor) {

        do {
          printf("Edad del menor: ");
          scanf("%d", &p.menor.edad);
        } while (p.menor.edad < 0 || p.menor.edad > 17);

        while (getchar() != '\n')
          ;

        printf("Nombre del menor: ");
        fgets(p.menor.nombre, sizeof(p.menor.nombre), stdin);
        p.menor.nombre[strcspn(p.menor.nombre, "\n")] =
            '\0'; // Eliminar el salto de línea al final
      }

      i = altaPaciente(ar, &p);

      if (i == 0) {
        printf("Paciente agregado correctamente.\n");
      } else if (i == 1) {
        printf("Error al agregar paciente.\n");
      } else if (i == 2) {
        printf("El consultorio ya esta ocupado. Prueba otro...\n");
      }

      break;

    case 2:

      subMenu();
      scanf("%c", &op2);
      do {
        printf("Ingrese una opcion:");
        scanf("%d", &op);
      } while (op2 != 'a' && op2 != 'b' && op2 != 'c');

      switch (op2) {

      case 'a':

        printf("Ingrese el ID del paciente a buscar: ");
        scanf("%d", &id);

        i = tomarPacienteID(ar, &p, id);
        if (i == 0) {
          printf("Paciente encontrado:\n");
          printf("ID: %d\nTipo: %d\nNombre: %s\nDireccion: %s\nEdad: "
                 "%d\nGenero: %d\nSintomas: %s\nConsultorio: %d\nEs Menor: "
                 "%d\nEdad del Menor: %d\nNombre del Menor: %s\n",
                 p.id, p.tipo, p.nombre, p.direccion, p.edad, p.genero,
                 p.sintomas, p.consultorio, p.esMenor, p.menor.edad,
                 p.menor.nombre);
        } else {
          printf("Paciente no encontrado.\n");
        }

        break;

      case 'b':

        while (getchar() != '\n')
          ;
        printf("Ingrese el nombre del paciente a buscar: ");
        fgets(nombre, sizeof(nombre), stdin);
        nombre[strcspn(nombre, "\n")] =
            '\0'; // Eliminar el salto de línea al final

        Paciente *pacientes = NULL;
        int cantidad = tomarPacienteNom(ar, &pacientes, nombre);

        if (cantidad > 0) {
          printf("Pacientes encontrados:\n");
          for (int j = 0; j < cantidad; j++) {
            printf("ID: %d\nTipo: %d\nNombre: %s\nDireccion: %s\nEdad: "
                   "%d\nGenero: %d\nSintomas: %s\nConsultorio: %d\nEs Menor: "
                   "%d\nEdad del Menor: %d\nNombre del Menor: %s\n",
                   pacientes[j].id, pacientes[j].tipo, pacientes[j].nombre,
                   pacientes[j].direccion, pacientes[j].edad,
                   pacientes[j].genero, pacientes[j].sintomas,
                   pacientes[j].consultorio, pacientes[j].esMenor,
                   pacientes[j].menor.edad, pacientes[j].menor.nombre);
          }
          free(pacientes);
        } else {
          printf("No se encontraron pacientes con ese nombre.\n");
        }

        break;

      case 'c':

        printf("Ingrese el ID del paciente a editar: ");
        scanf("%d", &id);

        p.id = id;

        do {
          printf("Edite el Tipo de Consulta (0=Externa, 1=Emergencia): ");
          scanf("%d", &p.tipo);
        } while (p.tipo < 0 || p.tipo > 1);

        while (getchar() != '\n')
          ;

        printf("Edite el Nombre: ");
        fgets(p.nombre, sizeof(p.nombre), stdin);
        p.nombre[strcspn(p.nombre, "\n")] = '\0';

        printf("Direccion: ");
        fgets(p.direccion, sizeof(p.direccion), stdin);
        p.direccion[strcspn(p.direccion, "\n")] = '\0';

        do {
          printf("Edite laEdad (Mayor de Edad): ");
          scanf("%d", &p.edad);
        } while (p.edad < 18 || p.edad > 130);

        do {
          printf("Edite el Genero (0=Masculino, 1=Femenino, 2=No Binario): ");
          scanf("%d", &p.genero);
        } while (p.genero < 0 || p.genero > 2);

        while (getchar() != '\n')
          ;

        printf("Edite los Sintomas: ");
        fgets(p.sintomas, sizeof(p.sintomas), stdin);
        p.sintomas[strcspn(p.sintomas, "\n")] =
            '\0'; // Eliminar el salto de línea al final

        if (p.tipo == 0) {
          do {
            printf("Edite el Consultorio (1-5): ");
            scanf("%d", &p.consultorio);
          } while (p.consultorio < 1 || p.consultorio > 5);
        } else {
          do {
            printf("Edite el Consultorio (6-10): ");
            scanf("%d", &p.consultorio);
          } while (p.consultorio < 6 || p.consultorio > 10);
        }

        do {
          printf("Edite si es menor de edad (1=si, 0=no): ");
          scanf("%d", &p.esMenor);
        } while (p.esMenor < 0 || p.esMenor > 1);

        if (p.esMenor) {

          do {
            printf("Edite la Edad del menor: ");
            scanf("%d", &p.menor.edad);
          } while (p.menor.edad < 0 || p.menor.edad > 17);

          while (getchar() != '\n')
            ;

          printf("Edite el Nombre del menor: ");
          fgets(p.menor.nombre, sizeof(p.menor.nombre), stdin);
          p.menor.nombre[strcspn(p.menor.nombre, "\n")] =
              '\0'; // Eliminar el salto de línea al final
        }

        i = editarPaciente(ar, id, p);
        if (i == 0) {
          printf("Paciente editado correctamente.\n");
        } else {
          printf("Error al editar paciente. ID no encontrado.\n");
        }
      }

    case 3:

      listarPacientes(ar);
      break;

    case 4:

      printf("Ingrese el ID del paciente a eliminar: ");
      scanf("%d", &id);

      i = removerPaciente(ar, id);
      if (i == 0) {
        printf("Paciente eliminado correctamente.\n");
      } else {
        printf("Error al eliminar paciente. ID no encontrado.\n");
      }

    default:

      printf("\nSaliendo del programa...\n");
      sentinela = 1;
    }

  } while (!sentinela);

  return 0;
}
