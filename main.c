#include "paclib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARCHIVO "data.txt"

int main() {

  FILE *ar;
  Paciente p;
  char nombre[100];
  int sentinela = 0, i, op, id;
  char op2;

  do {

    do {
      menu();
      if (scanf("%d", &op) != 1) {
        // Protección contra mamadas
        while (getchar() != '\n')
          ;
        op = 0;
      }
    } while (op < 1 || op > 5);

    switch (op) {

    case 1:

      printf("Ingrese los datos del paciente:\n");
      do {
        printf("ID: ");
        if (scanf("%d", &p.id) != 1) {
          while (getchar() != '\n')
            ;
          p.id = 0;
        }
      } while (p.id <= 0);

      while (getchar() != '\n')
        ;

      char sel;
      do {
        printf("Tipo de Consulta (E[x]terna, E[m]ergencia): ");
        scanf(" %c", &sel);
        switch (sel) {
        case 'x':
        case 'X':
          p.tipo = 0;
          break;
        case 'm':
        case 'M':
          p.tipo = 1;
          break;
        };
      } while (sel != 'x' && sel != 'm' && sel != 'X' && sel != 'M');

      while (getchar() != '\n')
        ;

      printf("Nombre: ");
      fgets(p.nombre, sizeof(p.nombre), stdin);
      p.nombre[strcspn(p.nombre, "\n")] = '\0';

      printf("Direccion: ");
      fgets(p.direccion, sizeof(p.direccion), stdin);
      p.direccion[strcspn(p.direccion, "\n")] = '\0';

      do {
        printf("Edad del paciente: ");
        if (scanf("%d", &p.edad) != 1) {
          while (getchar() != '\n')
            ;
          p.edad = 0;
        }
      } while (p.edad <= 0);

      do {
        printf("Género ([M]asculino, [F]emenino, No [B]inario): ");
        scanf(" %c", &sel);
        switch (sel) {
        case 'M':
        case 'm':
          p.genero = 0;
          break;
        case 'F':
        case 'f':
          p.genero = 1;
          break;
        case 'B':
        case 'b':
          p.genero = 2;
          break;
        };

      } while (sel != 'M' && sel != 'm' && sel != 'F' && sel != 'f' &&
               sel != 'B' && sel != 'b');

      while (getchar() != '\n')
        ;

      printf("Sintomas: ");
      fgets(p.sintomas, sizeof(p.sintomas), stdin);
      p.sintomas[strcspn(p.sintomas, "\n")] = '\0';

      if (p.tipo == 0) {
        do {
          printf("Consultorio (1-5): ");
          if (scanf("%d", &p.consultorio) != 1) {
            while (getchar() != '\n')
              ;
            p.consultorio = 0;
          }
        } while (p.consultorio < 1 || p.consultorio > 5);
      } else {
        do {
          printf("Consultorio (6-10): ");
          if (scanf("%d", &p.consultorio) != 1) {
            while (getchar() != '\n')
              ;
            p.consultorio = 0;
          }
        } while (p.consultorio < 6 || p.consultorio > 10);
      }

      if (p.edad < 13) {
        p.menor.edad = p.edad;
        p.esMenor = 1;
        strcpy(p.menor.nombre, p.nombre);

        do {
          printf("Edad del tutor: ");
          if (scanf("%d", &p.edad) != 1) {
            while (getchar() != '\n')
              ;
            p.edad = 0;
          }
        } while (p.edad < 18);

        while (getchar() != '\n')
          ;

        printf("Nombre del tutor: ");
        fgets(p.nombre, sizeof(p.nombre), stdin);
        p.nombre[strcspn(p.nombre, "\n")] = '\0';
      } else {
        p.esMenor = 0;
        p.menor.edad = 0;
        strcpy(p.menor.nombre, "NA");
      }

      ar = fopen(ARCHIVO, "a+");
      if (ar == NULL) {
        printf("Error al abrir el archivo.\n");
        break;
      }
      i = altaPaciente(ar, &p);
      fclose(ar);

      if (i == 0) {
        printf("Paciente agregado correctamente.\n");
      } else if (i == 2) {
        printf("El consultorio ya esta ocupado. Prueba otro...\n");
      } else if (i == 3) {
        printf("Error: Ya existe un paciente con el ID %d.\n", p.id);
      } else {
        printf("Error al agregar paciente.\n");
      }

      break;

    case 2:

      subMenu();
      do {
        scanf(" %c", &op2);
      } while (op2 != 'a' && op2 != 'b' && op2 != 'c' && op2 != 'A' &&
               op2 != 'B' && op2 != 'C');

      switch (op2) {

      case 'a':
      case 'A':

        printf("Ingrese el ID del paciente a buscar: ");
        if (scanf("%d", &id) != 1) {
          while (getchar() != '\n')
            ;
          printf("ID invalido.\n");
          break;
        }

        ar = fopen(ARCHIVO, "r");
        if (ar == NULL) {
          printf(
              "No hay pacientes registrados o no se pudo abrir el archivo.\n");
          break;
        }
        i = tomarPacienteID(ar, &p, id);
        fclose(ar);

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
      case 'B':

        while (getchar() != '\n')
          ;
        printf("Ingrese el nombre del paciente a buscar: ");
        fgets(nombre, sizeof(nombre), stdin);
        nombre[strcspn(nombre, "\n")] = '\0';

        Paciente *pacientes = NULL;
        ar = fopen(ARCHIVO, "r");
        if (ar == NULL) {
          printf(
              "No hay pacientes registrados o no se pudo abrir el archivo.\n");
          break;
        }
        int cantidad = tomarPacienteNom(ar, &pacientes, nombre);
        fclose(ar);

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
      case 'C':

        printf("Ingrese el ID del paciente a editar: ");
        if (scanf("%d", &id) != 1) {
          while (getchar() != '\n')
            ;
          printf("ID invalido.\n");
          break;
        }

        ar = fopen(ARCHIVO, "r");
        if (ar == NULL) {
          printf(
              "No hay pacientes registrados o no se pudo abrir el archivo.\n");
          break;
        }
        i = tomarPacienteID(ar, &p, id);
        fclose(ar);

        if (i != 0) {
          printf("Error: Paciente con ID %d no encontrado.\n", id);
          break;
        }

        p.id = id;

        do {
          printf("Edite tipo de Consulta (E[x]terna, E[m]ergencia): ");
          scanf(" %c", &sel);
          switch (sel) {
          case 'x':
          case 'X':
            p.tipo = 0;
            break;
          case 'm':
          case 'M':
            p.tipo = 1;
            break;
          };
        } while (sel != 'x' && sel != 'm' && sel != 'X' && sel != 'M');

        while (getchar() != '\n')
          ;

        p.esMenor ? printf("Edite el nombre del tutor: ")
                  : printf("Edite el nombre: ");
        fgets(p.nombre, sizeof(p.nombre), stdin);
        p.nombre[strcspn(p.nombre, "\n")] = '\0';

        printf("Edite dirección: ");
        fgets(p.direccion, sizeof(p.direccion), stdin);
        p.direccion[strcspn(p.direccion, "\n")] = '\0';

        do {
          p.esMenor ? printf("Edite la edad del tutor: ")
                    : printf("Edite la edad: ");
          if (scanf("%d", &p.edad) != 1) {
            while (getchar() != '\n')
              ;
            p.edad = 0;
          }
        } while (p.esMenor ? (p.edad < 18 || p.edad > 130)
                           : (p.edad <= 0 || p.edad > 130));

        do {
          printf("Edite género ([M]asculino, [F]emenino, No [B]inario): ");
          scanf(" %c", &sel);
          switch (sel) {
          case 'M':
          case 'm':
            p.genero = 0;
            break;
          case 'F':
          case 'f':
            p.genero = 1;
            break;
          case 'B':
          case 'b':
            p.genero = 2;
            break;
          };

        } while (sel != 'M' && sel != 'm' && sel != 'F' && sel != 'f' &&
                 sel != 'B' && sel != 'b');

        while (getchar() != '\n')
          ;

        printf("Edite los síntomas: ");
        fgets(p.sintomas, sizeof(p.sintomas), stdin);
        p.sintomas[strcspn(p.sintomas, "\n")] = '\0';

        if (p.tipo == 0) {
          do {
            printf("Edite el consultorio (1-5): ");
            if (scanf("%d", &p.consultorio) != 1) {
              while (getchar() != '\n')
                ;
              p.consultorio = 0;
            }
          } while (p.consultorio < 1 || p.consultorio > 5);
        } else {
          do {
            printf("Edite el consultorio (6-10): ");
            if (scanf("%d", &p.consultorio) != 1) {
              while (getchar() != '\n')
                ;
              p.consultorio = 0;
            }
          } while (p.consultorio < 6 || p.consultorio > 10);
        }

        if (p.esMenor) {
          do {
            printf("Edite la edad del menor: ");
            if (scanf("%d", &p.menor.edad) != 1) {
              while (getchar() != '\n')
                ;
              p.menor.edad = 0;
            }
          } while (p.menor.edad <= 0 || p.menor.edad > 17);

          while (getchar() != '\n')
            ;

          printf("Edite el nombre del menor: ");
          fgets(p.menor.nombre, sizeof(p.menor.nombre), stdin);
          p.menor.nombre[strcspn(p.menor.nombre, "\n")] = '\0';
        }

        ar = fopen(ARCHIVO, "r+");
        if (ar == NULL) {
          printf("Error al abrir el archivo.\n");
          break;
        }
        i = editarPaciente(ar, id, p);
        fclose(ar);

        if (i == 0) {
          printf("Paciente editado correctamente.\n");
        } else if (i == 2) {
          printf("El consultorio ya esta ocupado. Prueba otro...\n");
        } else {
          printf("Error al editar paciente. ID no encontrado.\n");
        }
        break;
      }
      break;

    case 3:

      ar = fopen(ARCHIVO, "r");
      if (ar == NULL) {
        printf("No hay pacientes registrados o no se pudo abrir el archivo.\n");
        break;
      }
      listarPacientes(ar);
      fclose(ar);
      break;

    case 4:

      printf("Ingrese el ID del paciente a eliminar: ");
      if (scanf("%d", &id) != 1) {
        while (getchar() != '\n')
          ;
        printf("ID invalido.\n");
        break;
      }

      ar = fopen(ARCHIVO, "r+");
      if (ar == NULL) {
        printf("No hay pacientes registrados o no se pudo abrir el archivo.\n");
        break;
      }
      i = removerPaciente(ar, id);
      fclose(ar);

      if (i == 0) {
        printf("Paciente eliminado correctamente.\n");
      } else {
        printf("Error al eliminar paciente. ID no encontrado.\n");
      }
      break;

    default:

      printf("\nSaliendo del programa...\n");
      sentinela = 1;
    }

  } while (!sentinela);

  return 0;
}
