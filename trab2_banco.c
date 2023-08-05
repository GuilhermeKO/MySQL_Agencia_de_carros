#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mysql/mysql.h>

void cadastro_setor(MYSQL *conexao, char *query);
void cadastro_carro(MYSQL *conexao, char *query, char *placa_cliente, int cod_setor);
void cadastro_funcionario(MYSQL *conexao, char *query);
void cadastro_cliente(MYSQL *conexao, char *query);
void registro_venda(MYSQL *conexao, char *query);
void registro_troca(MYSQL *conexao, char *query);
void registro_revisao(MYSQL *conexao, char *query);
void selection(MYSQL *conexao);
void error_check(MYSQL *conexao);

int main(){
	char query[2000], placa_cliente[50];
    int N, M;
	
	MYSQL *conexao = mysql_init(NULL);

	if (conexao == NULL){
	   	exit(1);
	}

	if (mysql_real_connect(conexao, "localhost", "root", "root", "Agencia", 0, NULL, 0) == NULL){
	    mysql_close(conexao);
		exit(1);
	}
    
    while(1){
    	system("clear");
    	printf("-------------------AGÊNCIA DE CARROS-------------------\n");
    	printf("\nDigite o número do comando que deseja realizar\n\n");
    	printf("1)Realizar Cadastro/Registro\n");
    	printf("2)Realizar Consulta/Atualização\n");
    	printf("0)Sair\n\n");
    	scanf("%d", &N);

    	if(N == 1){
    		system("clear");
	        while(1){
		        printf("Digite o número do comando que deseja realizar:\n");
		        printf("\n1)Cadastrar Setor\n");
		        printf("2)Cadastrar Carro\n");
		        printf("3)Cadastrar Funcionário\n");
		        printf("4)Cadastrar Cliente\n");
		        printf("5)Registrar Venda\n");
		        printf("6)Registrar Troca\n");
		        printf("7)Registrar Revisão\n");
		        printf("\n0)Voltar\n\n");

		        scanf("%d", &M);
		        system("clear");

		        if(M == 1){
		        	cadastro_setor(conexao, query);
		        } else if(M == 2){
		        	placa_cliente[0] = '\0';
		        	cadastro_carro(conexao, query, placa_cliente, 2);
		        } else if(M == 3){
		        	cadastro_funcionario(conexao, query);
		        } else if(M == 4){		        	  
		        	cadastro_cliente(conexao, query);
		        } else if(M == 5){
		        	registro_venda(conexao, query);
		        } else if(M == 6){
		        	registro_troca(conexao, query);
		        } else if(M == 7){
		        	registro_revisao(conexao, query);
		        } else {
		            break;
		      	}

	        }

	    } else if (N ==2){
			system("clear");
	        while(1){
		        printf("Digite o número do comando que deseja realizar:\n");
		        printf("\n1)Qual o modelo de carro mais vendido no ano;\n");
		        printf("2)Qual é o carro mais vendido de todos os tempos;\n");
		        printf("3)Quanto a agência lucrou em determinado periodo;\n");
		        printf("4)Quais clientes ja trocaram de carro mais de uma vez;\n");
		        printf("5)Quanto é a folha de pagamento mensal;\n");
		        printf("6)Quantos funcionários há em cada setor, e qual é o custo para manter esse setor;\n");
		        printf("7)Obter os clientes que estão com revisão atrasadas\n");
		        printf("8)Obter o gasto total por cliente na minha agência, com compra/troca de carro ou manutenções;\n");
		        printf("9)Determinar quantos carros (por modelo), e o valor total destes, estão armazenados na agência\n");
		        printf("10)Obter a lista de clientes que trocaram de carro há mais de 3 anos;\n");
		        printf("11)Descobrir quais funcionários são mais e menos eficientes (em venda de carros)\n");
		        printf("12)Aumentar o salário do funcionário do mês em 6 por cento\n");
		        printf("13)Demitir o funcionário com pior rendimento até um determinado momento.\n");
		        printf("14)Obter a lista de todos clientes, ordenados pelo gasto total que estes jé tiverem na agência;\n");
		        printf("15)Apagar os registros dos clientes que não efetuam negócio (troca de carro ou manutenção) há mais de 10 anos.\n");
		        printf("\n0)Voltar\n");

		        scanf("%d", &M);
		        system("clear");

		        if(M == 1){
					sprintf(query, "select modelo from (select max(qtd1) as maximo from (select count(modelo) as qtd1 from operacao join carro on placa = placa_carro_entrou where operacao.cod_setor = 1 and data_operacao > 20190101 and data_operacao < 20200101 group by modelo) as A) as B join (select modelo, count(modelo) as qtd2 from operacao join carro on placa = placa_carro_entrou where operacao.cod_setor = 1 and data_operacao > 20190101  and data_operacao < 20200101 group by modelo) as C on maximo = qtd2;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 2){
		        	sprintf(query, "select nome from (select max(qtd1) as maximo from (select count(modelo) as qtd1 from operacao join carro on placa = placa_carro_entrou where operacao.cod_setor = 1 group by nome_carro) as A) as B join (select nome_carro as nome, count(modelo) as qtd2 from operacao join carro on placa = placa_carro_entrou where operacao.cod_setor = 1 group by nome_carro) as C on maximo = qtd2;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 3){
		        	char dt_ini[10], dt_fim[10];
		        	printf("Informe a data inicial no formato (yyyymmdd): ");
		        	scanf("%s", dt_ini);
		        	printf("Informe a data final no formato (yyyymmdd): ");
		        	scanf(" %s", dt_fim);
		        	system("clear");
		        	printf("Valor em R$: ");
		        	sprintf(query, "select sum(valor_operacao) as Lucro from operacao where cod_setor = 1 or cod_setor = 2 and data_operacao > %s and data_operacao < %s ;", dt_ini, dt_fim);
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 4){		        	  
		        	char dt_ini[10], dt_fim[10];
		        	printf("Informe a data inicial no formato (yyyymmdd): ");
		        	scanf("%s", dt_ini);
		        	printf("Informe a data final no formato (yyyymmdd): ");
		        	scanf(" %s", dt_fim);
		        	system("clear");
		        	sprintf(query, "select nome_cliente from (select nome_cliente, count(nome_cliente) as vezes from operacao natural join cliente where cod_setor = 2 and data_operacao > %s and data_operacao < %s group by nome_cliente) as A where vezes > 1", dt_ini, dt_fim);
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 5){
		        	printf("Valor em R$: ");
		        	sprintf(query, "select sum(salario) as pagamento_mensal from funcionario;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 6){
		        	printf("Quantidade| Custo | Setor \n\n");
		        	sprintf(query, "select count(cpf_funcionario), sum(salario), nome_setor from funcionario natural join setor group by nome_setor;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 7){
		        	int ano;
		        	printf("Informe a data atual no formato(yymmdd): ");
		        	scanf("%d", &ano);
		        	ano = ano -10000;
		        	printf("CPF | Nome\n");
		        	sprintf(query, "select cpf_cliente, nome_cliente from operacao natural join cliente where operacao.cod_setor = 3 and data_operacao <= %d;", ano);
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 8){
		        	printf("CPF | Gasto | Nome\n\n");
		        	sprintf(query, "select cpf_cliente, sum(valor_operacao), nome_cliente as gasto from operacao natural join cliente group by(operacao.cpf_cliente);");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 9){		        	 
		        	printf("Quantidade | Valor Total | Modelo\n\n"); 
		        	sprintf(query, "select count(modelo), sum(valor), modelo from carro where situacao = \"disponivel\" group by modelo;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 10){
		        	int ano;
		        	printf("Informe a data atual no formato(yymmdd): ");
		        	scanf("%d", &ano);
		        	ano = ano - 30000;
		        	printf("CPF | Nome\n");
		        	sprintf(query, "select cpf_cliente, nome_cliente from operacao natural join cliente where operacao.cod_setor = 3 and data_operacao <= %d group by cpf_cliente;", ano);
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 11){
		        	printf("CPF | Qtd vendas | Nome \n\n"); 
		        	sprintf(query, "select cpf_funcionario, count(cpf_funcionario) as qtd_vendas, nome_funcionario from funcionario natural join operacao where cod_setor = 1 group by cpf_funcionario order by qtd_vendas desc;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 12){
		        	char dt_ini[10], dt_fim[10], cpf_f[50];
		        	printf("Informe a data inicial no formato (yyyymmdd): ");
		        	scanf("%s", dt_ini);
		        	printf("Informe a data final no formato (yyyymmdd): ");
		        	scanf(" %s", dt_fim);
		        	system("clear");

		        	MYSQL_RES *resultado;
					MYSQL_ROW row;

		        	sprintf(query, "select cpf_funcionario, sum(valor_operacao) as lucro from operacao where data_operacao > %s and data_operacao < %s group by cpf_funcionario order by lucro desc;", dt_ini, dt_fim);
					mysql_query(conexao, query);
					
					if(strlen(mysql_error(conexao)) == 0){
						resultado = mysql_use_result(conexao);
						row = mysql_fetch_row(resultado);
						if(row != NULL){
							sprintf(cpf_f, "%s", row[0]);
						}
					} else{
						printf("ERRO NO BANCO DE DADOS:\n");
						printf("\n%s\n\n", mysql_error(conexao));
						mysql_free_result(resultado);
						continue;
					}

					mysql_free_result(resultado);

					sprintf(query, "update funcionario set salario =  salario + ((salario*6)/100) where cpf_funcionario = \"%s\";", cpf_f);
					mysql_query(conexao, query);
					error_check(conexao);

		        } else if(M == 13){
		        	char ano[10], cpf_f1[50], cpf_f2[50];
		        	int setor;
		        	printf("Informe a data atual no formato(yyyymmdd)\n");
		        	scanf(" %s", ano);
		        	printf("Informe o codigo do setor\n");
		        	scanf(" %d", &setor);
					system("clear");

		        	sprintf(query, "select cpf_funcionario, sum(valor_operacao) as lucro from operacao where cod_setor = %d and data_operacao < %s group by cpf_funcionario order by lucro;", setor, ano);
					mysql_query(conexao, query);

					MYSQL_RES *resultado;
					MYSQL_ROW row;

					if(strlen(mysql_error(conexao)) == 0){
						resultado = mysql_use_result(conexao);
						row = mysql_fetch_row(resultado);
						if(row != NULL){
							sprintf(cpf_f1, "%s", row[0]);
						}

						row = mysql_fetch_row(resultado);
						if(row != NULL){
							sprintf(cpf_f2, "%s", row[0]);
						}

					} else{
						printf("ERRO NO BANCO DE DADOS:\n");
						printf("\n%s\n\n", mysql_error(conexao));
						mysql_free_result(resultado);
						continue;
					}

					mysql_free_result(resultado);

					sprintf(query, "update cliente set cpf_funcionario = \"%s\" where cpf_funcionario = \"%s\";", cpf_f2, cpf_f1);
					mysql_query(conexao, query);

					sprintf(query, "update operacao set cpf_funcionario = \"%s\" where cpf_funcionario = \"%s\";", cpf_f2, cpf_f1);
					mysql_query(conexao, query);

					sprintf(query, "delete from funcionario where cpf_funcionario = \"%s\";",cpf_f1);
					mysql_query(conexao, query);
					error_check(conexao);

		        } else if(M == 14){		        	  
					printf("CPF | Gasto Total | Nome \n\n"); 
		        	sprintf(query, "select cpf_cliente, sum(valor_operacao) as gasto_total, nome_cliente from cliente natural join operacao group by cpf_cliente order by gasto_total;");
					mysql_query(conexao, query);
					selection(conexao);
		        } else if(M == 15){
		        	printf("EM MANUTENÇÃO\n");
		        } else {
		        	break;
		        }
		    }

	    } else {
	       	system("clear");
	       	printf("bye\n");
	    	break;
	    }
	}
	
	mysql_close(conexao);
  
    return 0;
}

void selection(MYSQL *conexao){
	MYSQL_RES *resultado;
	MYSQL_ROW row;

	if(strlen(mysql_error(conexao)) == 0){
		resultado = mysql_use_result(conexao);
		int num_fields = mysql_num_fields(resultado);
    	while (((row = mysql_fetch_row(resultado)) != NULL)){
       		for(int i = 0; i < num_fields; i++){
				printf("%-10s ", row[i]);	
       		}
       		printf("\n");
    	}
    	printf("\n");
	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	mysql_free_result(resultado);


	printf("Digite 1 para continuar: ");
	int num = 0;
	while(num == 0){
		scanf("%d", &num);
	}
	system("clear");

	return;
}

void error_check(MYSQL *conexao){
	if(strlen(mysql_error(conexao)) == 0)
		printf("Operação realizada com sucesso!\n\n");
	else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
	}

	return;
}


void cadastro_setor(MYSQL *conexao, char *query){
	char nome_setor[50];
	printf("CADASTRO DE SETORES\n\n");
	printf("Informe o nome do setor:\n");
	scanf("%s", nome_setor);
	
	sprintf(query, "INSERT INTO setor(nome_setor) VALUES(\"%s\");", nome_setor);
	mysql_query(conexao, query);
	
	system("clear");

	error_check(conexao);

	return;
}


void cadastro_carro(MYSQL *conexao, char *query, char *placa_cliente, int cod_setor){
	char placa[50], nome_carro[50], modelo[50], valor[50], ano[50];

	printf("CADASTRO DE CARROS\n\n");
	printf("Informe:\n");
	
	if(placa_cliente[0] == '\0'){
		cod_setor = 1;
		printf("PLACA: ");
		scanf(" %s", placa);
		for(int i = 0; i < strlen(placa); ++i){
			placa[i] = toupper(placa[i]);
		}
	} else {
		strcpy(placa, placa_cliente);
	}
	
	printf("NOME DO CARRO: ");
	scanf(" %s", nome_carro);
	printf("MODELO: ");
	scanf(" %[^\n]", modelo);
	printf("VALOR: ");
	scanf(" %s", valor);
	printf("ANO: ");
	scanf(" %s", ano);
 	
 	sprintf(query, "INSERT INTO carro(placa, nome_carro, modelo, valor, ano, cod_setor) VALUES(\"%s\", \"%s\", \"%s\", %s, %s, %d);", placa, nome_carro, modelo, valor, ano, cod_setor);
	mysql_query(conexao, query);

	system("clear");
	
	error_check(conexao);

	return;
}

void cadastro_funcionario(MYSQL *conexao, char *query){
	char cpf_f[50], nome_f[50], end_f[50], salario[50], cod_setor[50];
	printf("CADASTRO DE FUNCIONÁRIOS\n\n");
	printf("Informe:\n");
	printf("CPF: ");
	scanf(" %s", cpf_f);
	printf("NOME: ");
	scanf(" %[^\n]", nome_f);
	printf("ENDEREÇO: ");
	scanf(" %[^\n]", end_f);
	printf("SALARIO: ");
	scanf(" %s", salario);
	printf("CODIGO SETOR: ");
	scanf(" %s", cod_setor);

	sprintf(query, "INSERT INTO funcionario(cpf_funcionario, nome_funcionario, endereco_funcionario, salario, cod_setor) VALUES(\"%s\", \"%s\", \"%s\", %s, %s);", cpf_f, nome_f, end_f, salario, cod_setor);
	mysql_query(conexao, query);
	
	system("clear");

	error_check(conexao);

	return;
}

void cadastro_cliente(MYSQL *conexao, char *query){
	char cpf_c[50], nome_c[50], tel[50], end_c[50], placa[50], cpf_f[50];
	int option;
	printf("CADASTRO DE CLIENTES\n\n");
	printf("Informe:\n");
	printf("CPF DO CLIENTE: ");
	scanf(" %s", cpf_c);
	printf("CPF DO FUNCIONÁRIO: ");
	scanf(" %s", cpf_f);		   
	printf("NOME DO CLIENTE: ");
	scanf(" %[^\n]", nome_c);
	printf("TELEFONE: ");
	scanf(" %s", tel);
	printf("ENDEREÇO: ");
	scanf(" %[^\n]", end_c);
	printf("POSSUI UM CARRO?  (1)SIM (2)NÃO\n");
	scanf("%d", &option);
	if(option == 1){	
		printf("PLACA DO CARRO: ");
		scanf(" %s", placa);
		for(int i = 0; i < strlen(placa); ++i){
			placa[i] = toupper(placa[i]);
		}

	} else {
		sprintf(placa, "NULL");
	}

	if(option == 1){
		sprintf(query, "INSERT INTO cliente(cpf_cliente, nome_cliente, telefone, placa_carro, cpf_funcionario, endereco_cliente) VALUES(\"%s\", \"%s\", %s, \"%s\", \"%s\", \"%s\");", cpf_c, nome_c, tel, placa, cpf_f, end_c);
	} else {
		sprintf(query, "INSERT INTO cliente(cpf_cliente, nome_cliente, telefone, placa_carro, cpf_funcionario, endereco_cliente) VALUES(\"%s\", \"%s\", %s, %s, \"%s\", \"%s\");", cpf_c, nome_c, tel, placa, cpf_f, end_c);
	}
	mysql_query(conexao, query);

	
	system("clear");
	error_check(conexao);
		   

	return;
}


void registro_venda(MYSQL *conexao, char *query){
	char data[50], cpf_f[50], cpf_c[50], placa[50], valor[50];
	sprintf(valor, "%s", "0");

	printf("REGISTRO DE VENDA\n\n");
	printf("Informe:\n");
	printf("DATA(yyyymmdd): ");
	scanf(" %s", data);
	printf("CPF DO CLIENTE: ");
	scanf(" %[^\n]", cpf_c);

	//Função para mostrar os carros disponíveis
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	sprintf(query, "select placa, nome_carro, modelo, valor from carro where situacao = \"disponivel\" order by nome_carro;");
	mysql_query(conexao, query);
	
	if(strlen(mysql_error(conexao)) == 0){
		printf("\nCARROS DISPONIVEIS:\n\n");
		resultado = mysql_use_result(conexao);
		int num_fields = mysql_num_fields(resultado);

		printf("PLACA%-11sNOME%-12sMODELO%-35sVALOR\n", "", "", "");
		printf("-----%-11s----%-12s------%-35s-----\n", "", "", "");
    	while (((row = mysql_fetch_row(resultado)) != NULL)){
       		for(int i = 0; i < num_fields; i++){
				if(i == 2){
					printf("%-40s ", row[i]);	
				} else 
					printf("%-15s ", row[i]);
       		}
       		printf("\n");
    	}
    	printf("\n");
	
	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	printf("PLACA DO CARRO: ");
	scanf(" %s", placa);


	//Função para pegar o cpf do funcionário da tabela cliente
	sprintf(query, "SELECT cpf_funcionario FROM cliente WHERE cpf_cliente = %s;", cpf_c);
	mysql_query(conexao, query);

	if(strlen(mysql_error(conexao)) == 0){
		resultado = mysql_use_result(conexao);
		row = mysql_fetch_row(resultado);
		if(row != NULL)
			sprintf(cpf_f, "%s", row[0]);
	
	} else{
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	mysql_free_result(resultado);

	MYSQL_RES *resultado2;
	MYSQL_ROW row2;

	//Funcao para pegar o valor do carro da tabela carro
	sprintf(query, "SELECT valor FROM carro WHERE placa = \"%s\";", placa);
	mysql_query(conexao, query);

	if(strlen(mysql_error(conexao)) == 0){
		resultado2 = mysql_use_result(conexao);
		row2 = mysql_fetch_row(resultado);
		if(row2 != NULL)
			sprintf(valor, "%s", row2[0]);
	
	} else{
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado2);
		return;
	}

	mysql_free_result(resultado2);

	sprintf(query, "INSERT INTO operacao(data_operacao, cod_setor, cpf_funcionario, cpf_cliente, placa_carro_entrou, valor_operacao) values (%s, 1, \"%s\", \"%s\", \"%s\", %s);", data, cpf_f, cpf_c, placa, valor); 
	mysql_query(conexao, query);

	system("clear");
	if(strlen(mysql_error(conexao)) == 0){
		//Atualiza o status do carro para vendido
		sprintf(query, "UPDATE carro SET situacao = \"vendido\" WHERE placa = \"%s\";", placa); 
		mysql_query(conexao, query);		   

		//Atualiza a placa do carro na tabela cliente
		sprintf(query, "UPDATE cliente SET placa_carro = \"%s\" where cpf_cliente = \"%s\";", placa, cpf_c); 
		mysql_query(conexao, query);		 
		error_check(conexao);

	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
	}

	return;
}

void registro_troca(MYSQL *conexao, char *query){
	char data[50], cpf_f[50], cpf_c[50], placa_cliente[50], placa_agencia[50], valor_c[50], valor_a[50];
	int ja_existe = 0;
	printf("Informe:\n");
	printf("DATA(yyyymmdd): ");
	scanf(" %s", data);
	printf("CPF DO CLIENTE: ");
	scanf(" %[^\n]", cpf_c);

	//Função para mostrar os carros disponíveis
	MYSQL_RES *resultado;
	MYSQL_ROW row;

	sprintf(query, "select placa, nome_carro, modelo, valor from carro where situacao = \"disponivel\" order by nome_carro;");
	mysql_query(conexao, query);

	if(strlen(mysql_error(conexao)) == 0){
		printf("\nCARROS DISPONIVEIS:\n\n");
		resultado = mysql_use_result(conexao);
		int num_fields = mysql_num_fields(resultado);

		printf("PLACA%-11sNOME%-12sMODELO%-35sVALOR\n", "", "", "");
		printf("-----%-11s----%-12s------%-35s-----\n", "", "", "");
    	while (((row = mysql_fetch_row(resultado)) != NULL)){
       		for(int i = 0; i < num_fields; i++){
				if(i == 2){
					printf("%-40s ", row[i]);	
				} else 
					printf("%-15s ", row[i]);
       		}
       		printf("\n");
    	}
    	printf("\n");
	
	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	printf("PLACA DO NOVO CARRO DO CLIENTE: ");
	scanf(" %[^\n]", placa_agencia);

	//Função para pegar os valores de cpf do funcionário e placa do carro associados ao cliente
	sprintf(query, "SELECT cpf_funcionario, placa_carro FROM cliente WHERE cpf_cliente = %s;", cpf_c);
	mysql_query(conexao, query);

	if(strlen(mysql_error(conexao)) == 0){
		resultado = mysql_use_result(conexao);
		row = mysql_fetch_row(resultado);
		if(row != NULL){
			sprintf(cpf_f, "%s", row[0]);
			sprintf(placa_cliente, "%s", row[1]);
		}
	
	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	mysql_free_result(resultado);

	
	sprintf(query, "INSERT INTO operacao(data_operacao, cod_setor, cpf_funcionario, cpf_cliente, placa_carro_entrou, placa_carro_saiu) values (%s, 2, \"%s\", \"%s\", \"%s\", \"%s\");", data, cpf_f, cpf_c, placa_cliente, placa_agencia); 
	mysql_query(conexao, query);
	
	system("clear");
	if(strlen(mysql_error(conexao)) == 0){
		
		//Atualiza o status do carro para trocado na tabela carro
		sprintf(query, "UPDATE carro SET situacao = \"trocado\" WHERE placa = \"%s\";", placa_agencia); 
		mysql_query(conexao, query);

		//Atualiza a placa do carro do cliente para a placa do novo carro
		sprintf(query, "UPDATE cliente SET placa_carro = \"%s\" where cpf_cliente = \"%s\";", placa_agencia, cpf_c); 
		mysql_query(conexao, query);		 
		error_check(conexao);

		//Função para verificar se o carro que está sendo adquirido pela agencia ja existe na tabela carro
		MYSQL_RES *resultado;
		MYSQL_ROW row;

		sprintf(query, "SELECT placa FROM carro where placa = \"%s\";", placa_cliente);
		mysql_query(conexao, query);


		if(strlen(mysql_error(conexao)) == 0){
			resultado = mysql_use_result(conexao);
			row = mysql_fetch_row(resultado);
			
			if(row != NULL){
				ja_existe = 1;
			}
		} else {
			return;
		}

		mysql_free_result(resultado);

		if(ja_existe == 0){
			printf("POR FAVOR CADASTRE O CARRO ANTIGO DO CLIENTE\n\n");
			cadastro_carro(conexao, query, placa_cliente, 2);
		} else {
			sprintf(query, "UPDATE carro SET situacao = \"disponivel\" WHERE placa = \"%s\";", placa_cliente); 
			mysql_query(conexao, query);
		}


		//Funcao para atualizar o valor da troca na tabela operacao
		MYSQL_RES *resultado2;
		MYSQL_ROW row2;

		sprintf(query, "SELECT valor FROM carro WHERE placa = \"%s\";", placa_cliente);
		mysql_query(conexao, query);

		if(strlen(mysql_error(conexao)) == 0){
			resultado2 = mysql_use_result(conexao);
			row2 = mysql_fetch_row(resultado2);
			
			if(row2 != NULL){
				sprintf(valor_c, "%s", row2[0]);
			}
		} else {
			printf("ERRO NO BANCO DE DADOS:\n");
			printf("\n%s\n\n", mysql_error(conexao));
			mysql_free_result(resultado2);
			return;
		}

		mysql_free_result(resultado2);

		MYSQL_RES *resultado3;
		MYSQL_ROW row3;

		sprintf(query, "SELECT valor FROM carro WHERE placa = \"%s\";", placa_agencia);
		mysql_query(conexao, query);


		if(strlen(mysql_error(conexao)) == 0){
			resultado3 = mysql_use_result(conexao);
			row3 = mysql_fetch_row(resultado3);
			
			if(row3 != NULL){
				sprintf(valor_a, "%s", row3[0]);
			}
		} else {
			printf("ERRO NO BANCO DE DADOS:\n");
			printf("\n%s\n\n", mysql_error(conexao));
			mysql_free_result(resultado3);
			return;
		}

		mysql_free_result(resultado3);
		

		double a, b, aux;
		sscanf(valor_c, "%lf", &a);
		sscanf(valor_a, "%lf", &b);

		aux = b - a;

		sprintf(query, "UPDATE operacao SET valor_operacao = %.2lf WHERE placa_carro_entrou = \"%s\" and cpf_cliente = \"%s\";", aux, placa_cliente, cpf_c); 
		mysql_query(conexao, query);


	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
	}

	return;
}

void registro_revisao(MYSQL *conexao, char *query){
	char cpf_c[50], cpf_f[50], placa[50], data[50];
	printf("informe:\n");
	printf("DATA(yyyymmdd): ");
	scanf(" %s", data);
	printf("CPF DO CLIENTE: ");
	scanf(" %[^\n]", cpf_c);
	printf("PLACA DO CARRO: ");
	scanf(" %[^\n]", placa);

	system("clear");
	printf("Por favor cadastre seu carro\n\n\n");
	cadastro_carro(conexao, query, placa, 3);

	MYSQL_RES *resultado;
	MYSQL_ROW row;

	sprintf(query, "SELECT cpf_funcionario FROM cliente WHERE cpf_cliente = %s;", cpf_c);
	mysql_query(conexao, query);

	if(strlen(mysql_error(conexao)) == 0){
		resultado = mysql_use_result(conexao);
		row = mysql_fetch_row(resultado);
		if(row != NULL){
			sprintf(cpf_f, "%s", row[0]);
		}

	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
		mysql_free_result(resultado);
		return;
	}

	mysql_free_result(resultado);

	sprintf(query, "INSERT INTO operacao(data_operacao, cod_setor, cpf_funcionario, cpf_cliente, placa_carro_entrou, valor_operacao) values (%s, 3, \"%s\", \"%s\", \"%s\", 250.00);", data, cpf_f, cpf_c, placa); 
	mysql_query(conexao, query);
	
	system("clear");
	if(strlen(mysql_error(conexao)) == 0){
		sprintf(query, "UPDATE carro SET situacao = \"revisao\" WHERE placa = \"%s\";", placa); 
		mysql_query(conexao, query);
		error_check(conexao);

	} else {
		printf("ERRO NO BANCO DE DADOS:\n");
		printf("\n%s\n\n", mysql_error(conexao));
	}	


	return;
}