/*
 * ******************************************************************
 * ZYNTHIAN PROJECT: Zynmidirouter Library
 * 
 * MIDI router library: Implements the MIDI router & filter 
  * 
 * Copyright (C) 2015-2016 Fernando Moyano <jofemodo@zynthian.org>
 *
 * ******************************************************************
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the LICENSE.txt file.
 * 
 * ******************************************************************
 */

//-----------------------------------------------------------------------------
// Library Initialization
//-----------------------------------------------------------------------------

int init_zynmidirouter();
int end_zynmidirouter();

//-----------------------------------------------------------------------------
// MIDI Router Data Structures
//-----------------------------------------------------------------------------

enum midi_event_type_enum {
	//Router-internal pseudo-message codes
	SWAP_EVENT=-3,
	IGNORE_EVENT=-2,
	THRU_EVENT=-1,
	//Channel 3-bytes-messages
	NOTE_OFF=0x8,
	NOTE_ON=0x9,
	KEY_PRESS=0xA,
	CTRL_CHANGE=0xB,
	PITCH_BENDING=0xE,
	//Channel 2-bytes-messages
	PROG_CHANGE=0xC,
	CHAN_PRESS=0xD,
	//System 2-bytes-messages
	TIME_CODE_QF=0xF1,
	SONG_SELECT=0xF3,
	//System 1-byte messages
	ACTIVE_SENSE=0xFE,
	//System Multi-byte (SysEx)
	SYSTEM_EXCLUSIVE=0xF0
};

struct midi_event_st {
	enum midi_event_type_enum type;
	uint8_t chan;
	uint8_t num;
};

struct mf_arrow_st {
	uint8_t chan_from;
	uint8_t num_from;
	uint8_t chan_to;
	uint8_t num_to;
	enum midi_event_type_enum type;
};

struct midi_filter_st {
	int tuning_pitchbend;
	int transpose[16];
	struct midi_event_st event_map[8][16][128];

	int master_chan;
	uint8_t last_ctrl_val[16][128];
	uint16_t last_pb_val[16];
};
struct midi_filter_st midi_filter;

//-----------------------------------------------------------------------------
// MIDI Router Functions
//-----------------------------------------------------------------------------

//MIDI filter initialization
int init_midi_router();
int end_midi_router();

void set_midi_master_chan(int chan);

//MIDI filter fine tuning => Pitch-Bending based
void set_midi_filter_tuning_freq(int freq);
int get_midi_filter_tuning_pitchbend();

//MIDI filter transpose
void set_midi_filter_transpose(uint8_t chan, int offset);
int get_midi_filter_transpose(uint8_t chan);

//MIDI Filter Core functions
void set_midi_filter_event_map_st(struct midi_event_st *ev_from, struct midi_event_st *ev_to);
void set_midi_filter_event_map(enum midi_event_type_enum type_from, uint8_t chan_from, uint8_t num_from,
															 enum midi_event_type_enum type_to, uint8_t chan_to, uint8_t num_to);
void set_midi_filter_event_ignore_st(struct midi_event_st *ev_from);
void set_midi_filter_event_ignore(enum midi_event_type_enum type_from, uint8_t chan_from, uint8_t num_from);
struct midi_event_st *get_midi_filter_event_map_st(struct midi_event_st *ev_from);
struct midi_event_st *get_midi_filter_event_map(enum midi_event_type_enum type_from, uint8_t chan_from, uint8_t num_from);
void del_midi_filter_event_map_st(struct midi_event_st *ev_filter);
void del_midi_filter_event_map(enum midi_event_type_enum type_from, uint8_t chan_from, uint8_t num_from);
void reset_midi_filter_event_map();

//MIDI Filter Mapping
void set_midi_filter_cc_map(uint8_t chan_from, uint8_t cc_from, uint8_t chan_to, uint8_t cc_to);
void set_midi_filter_cc_ignore(uint8_t chan, uint8_t cc_from);
uint8_t get_midi_filter_cc_map(uint8_t chan, uint8_t cc_from);
void del_midi_filter_cc_map(uint8_t chan, uint8_t cc_from);
void reset_midi_filter_cc_map();

//MIDI Filter Swap Mapping
int get_mf_arrow_from(enum midi_event_type_enum type, uint8_t chan, uint8_t num, struct mf_arrow_st *arrow);
int get_mf_arrow_to(enum midi_event_type_enum type, uint8_t chan, uint8_t num, struct mf_arrow_st *arrow);
int set_midi_filter_cc_swap(uint8_t chan_from, uint8_t num_from, uint8_t chan_to, uint8_t num_to);
int del_midi_filter_cc_swap(uint8_t chan, uint8_t num);
uint8_t get_midi_filter_cc_swap(uint8_t chan, uint8_t num);

//-----------------------------------------------------------------------------
// Jack MIDI
//-----------------------------------------------------------------------------

int init_jack_midi(char *name);
int end_jack_midi();

//-----------------------------------------------------------------------------
// MIDI Input Events Buffer Management
//-----------------------------------------------------------------------------

#define ZYNMIDI_BUFFER_SIZE 32
uint32_t zynmidi_buffer[ZYNMIDI_BUFFER_SIZE];
int zynmidi_buffer_read;
int zynmidi_buffer_write;

int write_zynmidi(uint32_t ev);
uint32_t read_zynmidi();

//-----------------------------------------------------------------------------
// MIDI Send Functions
//-----------------------------------------------------------------------------

int zynmidi_send_note_off(uint8_t chan, uint8_t note, uint8_t vel);
int zynmidi_send_note_on(uint8_t chan, uint8_t note, uint8_t vel);
int zynmidi_send_ccontrol_change(uint8_t chan, uint8_t ctrl, uint8_t val);
int zynmidi_send_program_change(uint8_t chan, uint8_t prgm);
int zynmidi_send_pitchbend_change(uint8_t chan, uint16_t pb);

int zynmidi_send_master_ccontrol_change(uint8_t ctrl, uint8_t val);

//-----------------------------------------------------------------------------