#ifndef __POINT_TO_POINT_TRANSMITTER_H__
#define __POINT_TO_POINT_TRANSMITTER_H__

#define WAKE_UP_SEQUENCE_LENGTH_MAX 18

#include "PointToPointBase.h"
#include "RadioPlaner.h"
#include "sx1276.h"

class PointToPointTransmitter
{
public:
  explicit PointToPointTransmitter(RadioPLaner<SX1276> *radio_planner, const uint8_t hook_id);
  ~PointToPointTransmitter();

  void Start(uint8_t *data_payload, const uint8_t data_payload_length);
  void Abort();
  void GetStatistics(StatisticCounters_t *counters);
  static void Callback(void *);

protected:

  void ExecuteStateMachine();
  void PrepareNextWakeUpFragment(WakeUpFragments_t *fragment, const uint8_t fragment_index);
  static void ComputeNextWakeUpLength(uint8_t *nextWakeUpLength, const uint32_t actual_time, const uint32_t last_ack_success_time);

  static void GetNextSendSlotTimeAndChannel(const uint32_t actual_time, const int16_t delay_rx, const uint32_t last_ack_success_time, uint8_t *wake_up_sequence_length, uint32_t *next_send_slot, uint8_t *channel_index);

private:
  RadioPLaner<SX1276> *radio_planner;
  const uint8_t hook_id;

  typedef enum
  {
    STATE_INIT,
    STATE_SEND_WAKEUP_SEQUENCE_FRAGMENT,
    STATE_WAIT_RELAY_ACK,
    ACK_RECEIVED
  } State_t;

  State_t state;

  uint32_t SendWakeUpCount;
  volatile int16_t WakeUpSequenceDelay;
  volatile uint32_t last_ack_success_received_ms;
  volatile uint32_t irq_timestamp_ms;

  uint8_t WakeUpSequenceLength;
  uint32_t NextSendSlot;
  uint8_t ChannelIndex;
  uint32_t TxChannel;
  uint32_t FrequencyList[NBR_FREQUENCIES];
  uint8_t DataSf;
  eBandWidth DataBw;
  volatile bool RxDone;
  volatile bool TxDone;
  WakeUpFragments_t fragment;
  uint8_t fragment_length;
  uint8_t fragment_index;
  Acknowledges_t ack;
  uint8_t rx_buffer[2];
  uint8_t ack_length;
  uint8_t *data_payload;
  uint8_t data_payload_length;
  SRadioParam wakeup_fragment_task_param;
  STask wakeup_fragment_task;
  SRadioParam ack_relay_rx_task_param;
  STask ack_relay_rx_task;
  SRadioParam data_send_task_param;
  STask data_send_task;
  volatile bool rxSuccess;

  uint32_t count_wus_tx_attempt;
  uint32_t count_wuf_tx_attempt;
  uint32_t count_data_tx_attempt;
  uint32_t count_ack_rx_attempt;
  uint32_t count_ack_rx_success;
};

#endif // __POINT_TO_POINT_TRANSMITTER_H__