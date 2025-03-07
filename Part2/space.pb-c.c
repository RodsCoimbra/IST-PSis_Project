/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: space.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "space.pb-c.h"
void   score_update__init
                     (ScoreUpdate         *message)
{
  static const ScoreUpdate init_value = SCORE_UPDATE__INIT;
  *message = init_value;
}
size_t score_update__get_packed_size
                     (const ScoreUpdate *message)
{
  assert(message->base.descriptor == &score_update__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t score_update__pack
                     (const ScoreUpdate *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &score_update__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t score_update__pack_to_buffer
                     (const ScoreUpdate *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &score_update__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ScoreUpdate *
       score_update__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ScoreUpdate *)
     protobuf_c_message_unpack (&score_update__descriptor,
                                allocator, len, data);
}
void   score_update__free_unpacked
                     (ScoreUpdate *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &score_update__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   score_board__init
                     (ScoreBoard         *message)
{
  static const ScoreBoard init_value = SCORE_BOARD__INIT;
  *message = init_value;
}
size_t score_board__get_packed_size
                     (const ScoreBoard *message)
{
  assert(message->base.descriptor == &score_board__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t score_board__pack
                     (const ScoreBoard *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &score_board__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t score_board__pack_to_buffer
                     (const ScoreBoard *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &score_board__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ScoreBoard *
       score_board__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ScoreBoard *)
     protobuf_c_message_unpack (&score_board__descriptor,
                                allocator, len, data);
}
void   score_board__free_unpacked
                     (ScoreBoard *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &score_board__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor score_update__field_descriptors[2] =
{
  {
    "astronaut",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(ScoreUpdate, astronaut),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "points",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(ScoreUpdate, points),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned score_update__field_indices_by_name[] = {
  0,   /* field[0] = astronaut */
  1,   /* field[1] = points */
};
static const ProtobufCIntRange score_update__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor score_update__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ScoreUpdate",
  "ScoreUpdate",
  "ScoreUpdate",
  "",
  sizeof(ScoreUpdate),
  2,
  score_update__field_descriptors,
  score_update__field_indices_by_name,
  1,  score_update__number_ranges,
  (ProtobufCMessageInit) score_update__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor score_board__field_descriptors[1] =
{
  {
    "scores",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ScoreBoard, n_scores),
    offsetof(ScoreBoard, scores),
    &score_update__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned score_board__field_indices_by_name[] = {
  0,   /* field[0] = scores */
};
static const ProtobufCIntRange score_board__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor score_board__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ScoreBoard",
  "ScoreBoard",
  "ScoreBoard",
  "",
  sizeof(ScoreBoard),
  1,
  score_board__field_descriptors,
  score_board__field_indices_by_name,
  1,  score_board__number_ranges,
  (ProtobufCMessageInit) score_board__init,
  NULL,NULL,NULL    /* reserved[123] */
};
