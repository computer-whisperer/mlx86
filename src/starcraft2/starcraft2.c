#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "starcraft2_buildspec.h"

#include "s2clientprotocol/sc2api.pb-c.h"

#include "types.h"
#include "utils.h"

#define PORT_BASE 5000

pid_t sc2_pid;

U32 start_sc2()
{
	U8 * portstring[10];
	sprintf(portstring, "%d", PORT_BASE + get_process_offset());

	// Setup stderr and stdout redirects
	int link[2];
	pipe(link);


	if (0 == (sc2_pid = fork()))
	{
		dup2 (link[1], STDERR_FILENO);
		close(link[0]);
		close(link[1]);
		U8 ret = execl("src/starcraft2/StarCraftII/Versions/Base59877/SC2_x64",
				"src/starcraft2/StarCraftII/Versions/Base59877/SC2_x64",
				"-listen" , "127.0.0.1" , "-port", portstring, NULL);
		if (-1 == ret)
		{
	                    perror("child process execve failed [%m]");
		}
		exit(0);
	}
	close(link[1]);

	// Wait for complete or fail message

	while (1)
	{
		U8 buff[1000];
		U32 bytes_read = 0;
		// Read line
	    while (1)
	    {
	    	bytes_read += read(link[0], &(buff[bytes_read]), 1);
	    	if (bytes_read >= 999)
	    	{
	    		break;
	    	}
	    	if (buff[bytes_read-1] == '\n')
	    	{
	    		break;
	    	}
	    }
	    buff[bytes_read] = 0;
	    //Version: B59877 (SC2.4.0)
	    if (bytes_read == 26)
	    {
	    }
	    //Build: Nov 27 2017 21:52:46
	    else if (bytes_read == 28)
	    {
	    }
	    //Starting up...
	    else if (bytes_read == 15)
	    {
	    }
	    //Startup Phase 1 complete
	    //Startup Phase 2 complete
	    else if (bytes_read == 25)
	    {
	    }
	    //Command Line: '"src/starcraft2/StarCraftII/Versions/Base59877/SC2_x64" -listen 127.0.0.1 -port 5000'
	    else if (bytes_read == 101)
	    {
	    }
	    //Listening on: 127.0.0.1:5000 (5000)
	    else if (bytes_read == 36)
	    {
	    }
	    // Startup Phase 3 complete. Ready for commands.
	    else if (bytes_read == 46)
	    {
	    	break;
	    }
	    else
	    {
	    	printf("%d chars long:", bytes_read);
	    	printf(buff);
	    }
	}


    U32 sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT_BASE + get_process_offset());
    U32 status = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (status != 0)
    {
    	fprintf(stderr, "Connect error!\n");
    	return 0;
    }

    const U8 * opening_tx = "GET /sc2api HTTP/1.1\r\n"
    		"Upgrade: websocket\r\n"
    		"Host: 127.0.0.1\r\n"
    		"Origin: http://example.com\r\n"
    		"Connection: Upgrade\r\n"
    		"Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
    		"Sec-WebSocket-Protocol: chat, superchat\r\n"
            "Sec-WebSocket-Version: 13\r\n\r\n";
    U32 bytes_sent = send(sockfd, opening_tx, strlen(opening_tx), 0);
    if (bytes_sent < strlen(opening_tx))
    {
    	fprintf(stderr, "Problem with opening tx!\n");
    	return 0;
    }

    return sockfd;
}

void stop_sc2()
{
	kill(sc2_pid, SIGTERM);
}

void send_ws_data(U32 sockfd, SC2APIProtocol__Request * req)
{
    U32 len = sc2_apiprotocol__request__get_packed_size(req);
    U8 * tx_buff = malloc(len);
    sc2_apiprotocol__request__pack(req, tx_buff);

	U8 buffer[1000];
	U32 buff_len = 0;
	if (len > 125)
	{
		printf("Too much data! Must revise ws sender.\n");
		return;
	}

	buffer[0] = 0x82; // 1 fin, 2 opcode
	buffer[1] = 0x80 | (len&0x7F); // 1 mask, data
	buffer[2] = 0x00; // mask
	buffer[3] = 0x00; // mask
	buffer[4] = 0x00; // mask
	buffer[5] = 0x00; // mask
	buff_len = 6;
	memcpy(&(buffer[buff_len]), tx_buff, len);
	buff_len += len;

	U32 bytes_sent = send(sockfd, buffer, buff_len, 0);
	if (bytes_sent != buff_len)
	{
		printf("Problem with ws sender.\n");
	}
}

U8 is_binary_protocol = 0;

SC2APIProtocol__Response * recv_ws_data(U32 sockfd)
{
	U8 rxbuffer[1000];
	while (1)
	{
		if (!is_binary_protocol)
		{
			U32 bytes_received = 0;

			while (1)
			{
				bytes_received += recv(sockfd, &(rxbuffer[bytes_received]), 1, 0);

				if (rxbuffer[bytes_received-1] == '\n')
				{
					//HTTP/1.1 101 Switching Protocols
					if ((bytes_received == 34) && (rxbuffer[31] == 's'))
					{
					}
					//Upgrade: websocket
					else if ((bytes_received == 20) && (rxbuffer[17] == 't'))
					{
					}
					//Connection: Upgrade
					else if ((bytes_received == 21) && (rxbuffer[18] == 'e'))
					{
					}
					//Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=
					else if ((bytes_received == 52) && (rxbuffer[48] == 'k'))
					{
					}
					//Sec-WebSocket-Protocol: chat
					else if ((bytes_received == 30) && (rxbuffer[24] == 'c'))
					{
					}
					//\r\n
					else if ((bytes_received == 2))
					{
						is_binary_protocol = 1;
					}
					else
					{
						rxbuffer[bytes_received] = 0;
						printf("Unknown text response:\n%s\n", rxbuffer);
					}
					break;
				}
			}
		}
		else
		{
			U32 bytes_received = 0;
			while (bytes_received < 2)
			{
				bytes_received += recv(sockfd, &(rxbuffer[bytes_received]), 1, 0);
			}
			U8 final = (rxbuffer[0]&0x80)>>7;
			U8 opcode = rxbuffer[0]&0x0F;
			U32 len = rxbuffer[1]&0x7F;
			U8 mask = (rxbuffer[1]&0x80)>>7;

			if (!final)
			{
				printf("Multi packet message, not ready for this!\n");
			}

			if (len == 126)
			{
				// 16 bit payload len
				bytes_received = 0;
				while (bytes_received < 2)
				{
					bytes_received += recv(sockfd, &(rxbuffer[bytes_received]), 1, 0);
				}
				len = (rxbuffer[0]<<8) | rxbuffer[1];
			}

			if (len == 127)
			{
				// 64 bit payload len
				bytes_received = 0;
				while (bytes_received < 8)
				{
					bytes_received += recv(sockfd, &(rxbuffer[bytes_received]), 1, 0);
				}
				len = (rxbuffer[0]<<24) | (rxbuffer[1]<<16) | (rxbuffer[2]<<8) | rxbuffer[3];
			}

			bytes_received = 0;
			while (bytes_received < len)
			{
				bytes_received += recv(sockfd, &(rxbuffer[bytes_received]), 1, 0);
			}

			if (opcode == 0xA)
			{
				// Got a pong, redo
				printf("Pong.\n");
				continue;
			}
			else if (opcode == 0x2)
			{
				// Binary frame
				if (mask)
				{
					printf("Bad mask, you should not be there.\n");
				}

			    SC2APIProtocol__Response * resp = sc2_apiprotocol__response__unpack(NULL, len,  &(rxbuffer));
			    if (resp == NULL)
			    {
			      fprintf(stderr, "error unpacking incoming message\n");
			      print_data_as_hex(rxbuffer, bytes_received);
			    }

				return resp;
			}
			else
			{
				printf("Got an unknown opcode: %d\n", opcode);
				print_data_as_hex(rxbuffer, bytes_received);
				return 0;
			}
		}
	}
}

int create_game(U32 sockfd)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestCreateGame req2 = SC2_APIPROTOCOL__REQUEST_CREATE_GAME__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_CREATE_GAME;
    req.create_game = &req2;
    req2.map_case = SC2_APIPROTOCOL__REQUEST_CREATE_GAME__MAP_LOCAL_MAP;
    req2.disable_fog = 0;
    req2.has_disable_fog = 1;
    req2.realtime = 0;
    req2.has_realtime = 1;
    req2.random_seed = 0;
    req2.has_random_seed = 1;

    SC2APIProtocol__LocalMap m = SC2_APIPROTOCOL__LOCAL_MAP__INIT;
    m.map_path = "Acropolis\ LE\ no\ victory\ and\ no\ ai\ ai.SC2Map";

    SC2APIProtocol__PlayerSetup p1 = SC2_APIPROTOCOL__PLAYER_SETUP__INIT;
    p1.type = SC2_APIPROTOCOL__PLAYER_TYPE__Participant;
    p1.has_type = 1;

    SC2APIProtocol__PlayerSetup p2 = SC2_APIPROTOCOL__PLAYER_SETUP__INIT;
    p2.type = SC2_APIPROTOCOL__PLAYER_TYPE__Computer;
    p2.has_type = 1;
    p2.difficulty = SC2_APIPROTOCOL__DIFFICULTY__VeryEasy;
    p2.has_difficulty = 1;
    p2.race = SC2_APIPROTOCOL__RACE__Protoss;
    p2.has_race = 1;
    p2.player_name = "AI";

    SC2APIProtocol__PlayerSetup * ps[2] = {&p1, &p2};
    req2.player_setup = ps;
    req2.n_player_setup = 1;
    req2.local_map = &m;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_CREATE_GAME))
    {
    	if (resp->create_game->has_error)
    	{
    		fprintf(stderr, "%s\n", resp->create_game->error_details);
    		ret = 1;
    	}
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int shutdown_sc2(U32 sockfd)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestQuit req2 = SC2_APIPROTOCOL__REQUEST_QUIT__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_QUIT;
    req.quit = &req2;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_QUIT))
    {
    	ret = 0;
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int join_game(U32 sockfd)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestJoinGame req2 = SC2_APIPROTOCOL__REQUEST_JOIN_GAME__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_JOIN_GAME;
    req.join_game = &req2;
    req2.participation_case = SC2_APIPROTOCOL__REQUEST_JOIN_GAME__PARTICIPATION_RACE;
    req2.race = SC2_APIPROTOCOL__RACE__Protoss;

    SC2APIProtocol__InterfaceOptions opt = SC2_APIPROTOCOL__INTERFACE_OPTIONS__INIT;
    req2.options = &opt;
    opt.raw = 0;
    opt.has_raw = 1;
    opt.score = 0;
    opt.has_score = 1;
    opt.show_burrowed_shadows = 0;
    opt.has_show_burrowed_shadows = 1;
    opt.show_placeholders = 1;
    opt.has_show_placeholders = 1;
    opt.show_cloaked = 0;
    opt.has_show_cloaked = 1;
    opt.raw_affects_selection = 0;
    opt.has_raw_affects_selection = 1;
    opt.raw_crop_to_playable_area = 1;
    opt.has_raw_crop_to_playable_area = 1;
    req2.player_name = "mlx86";

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_JOIN_GAME))
    {
    	if (resp->join_game->has_error)
    	{
    		fprintf(stderr, "error: %d\n", resp->join_game->error);
    		fprintf(stderr, "%s\n", resp->join_game->error_details);
    		ret=1;
    	}
    }
    else
    {
    	ret=1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int leave_game(U32 sockfd)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestLeaveGame req2 = SC2_APIPROTOCOL__REQUEST_LEAVE_GAME__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_LEAVE_GAME;
    req.step = &req2;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_LEAVE_GAME))
    {
    	ret = 0;
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int restart_game(U32 sockfd)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestRestartGame req2 = SC2_APIPROTOCOL__REQUEST_RESTART_GAME__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_RESTART_GAME;
    req.step = &req2;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_RESTART_GAME))
    {
    	ret = 0;
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int request_step(U32 sockfd, U32 step_count)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestStep req2 = SC2_APIPROTOCOL__REQUEST_STEP__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_STEP;
    req.step = &req2;
    req2.has_count = 1;
    req2.count = step_count;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_STEP))
    {
    	ret = 0;
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

int request_action(U32 sockfd, SC2APIProtocol__Action * action)
{

    SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
    SC2APIProtocol__RequestAction req2 = SC2_APIPROTOCOL__REQUEST_ACTION__INIT;
    req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_ACTION;
    req.step = &req2;
    req2.n_actions = 1;

    SC2APIProtocol__Action * actions[1] = {action};
    req2.actions = actions;

    send_ws_data(sockfd, &req);

    SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
    U8 ret = 0;
    if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_ACTION))
    {
    	ret = 0;
    }
    else
    {
    	ret = 1;
    }
    sc2_apiprotocol__response__free_unpacked(resp, NULL);
    return ret;
}

static U32 sockfd = 0;
void starcraft2_run_build(struct StarCraft2BuildStep * build, U32 num_build_steps)
{
	if (sockfd == 0)
	{
		sockfd = start_sc2();

	    if (create_game(sockfd))
	    {
	    	printf("Problems with creating a game.\n");
	    }

	    if (join_game(sockfd))
	    {
	    	printf("Problems with joining a game.\n");
	    }
	}
	restart_game(sockfd);

	U32 build_step = 0;

	request_step(sockfd, build[build_step].wait+1);

    while (1)
    {
		SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
		SC2APIProtocol__RequestObservation req2 = SC2_APIPROTOCOL__REQUEST_OBSERVATION__INIT;
		req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_OBSERVATION;
		req.observation = &req2;
		req2.has_disable_fog = 1;
		req2.disable_fog = 0;

		send_ws_data(sockfd, &req);

		SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
		if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_OBSERVATION))
		{
			if (resp->has_status)
			{
				if (resp->status == SC2_APIPROTOCOL__STATUS__ended)
				{
					printf("Game has ended.\n");
					break;
				}
			}

			if (build[build_step].command == StarCraft2BuildCommand_BuildProbe)
			{

			}
			else if (build[build_step].command == StarCraft2BuildCommand_BuildNexus)
			{

			}
			else if (build[build_step].command == StarCraft2BuildCommand_BuildPylon)
			{

			}
			else if (build[build_step].command == StarCraft2BuildCommand_BuildGateway)
			{

			}
			else if (build[build_step].command == StarCraft2BuildCommand_BuildZealot)
			{

			}

			build_step++;

			if (build_step >=  num_build_steps)
			{
				break;
			}

			request_step(sockfd, build[build_step].wait+1);
		}
		else
		{
			fprintf(stderr, "Bad observation response.\n");
			break;
		}
		sc2_apiprotocol__response__free_unpacked(resp, NULL);
    }
}

double starcraft2_check_supplymax_time()
{
	double res = 0;
    while (1)
    {
		SC2APIProtocol__Request req = SC2_APIPROTOCOL__REQUEST__INIT;
		SC2APIProtocol__RequestObservation req2 = SC2_APIPROTOCOL__REQUEST_OBSERVATION__INIT;
		req.request_case = SC2_APIPROTOCOL__REQUEST__REQUEST_OBSERVATION;
		req.observation = &req2;
		req2.has_disable_fog = 1;
		req2.disable_fog = 0;

		send_ws_data(sockfd, &req);

		SC2APIProtocol__Response * resp = recv_ws_data(sockfd);
		if (resp && (resp->response_case == SC2_APIPROTOCOL__RESPONSE__RESPONSE_OBSERVATION))
		{
			if (resp->has_status)
			{
				if (resp->status == SC2_APIPROTOCOL__STATUS__ended)
				{
					printf("Game has ended.\n");
					break;
				}
			}
			if (resp->observation->observation->player_common->food_used >= 200)
			{
				res = resp->observation->observation->game_loop/20;
				break;
			}
			if (resp->observation->observation->game_loop >= 10*60*20)
			{
				break;
			}
			else
			{
				request_step(sockfd, 20);
			}
		}
		else
		{
			fprintf(stderr, "Bad observation response.\n");
			break;
		}
		sc2_apiprotocol__response__free_unpacked(resp, NULL);
    }
    return res;
}
