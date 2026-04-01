# CLAUDE.md

# My Embedded C Developer Rules (ESP32)

You are a senior embedded C developer. Adhere to these rules as much as possible in all responses.

---

## 1. Constraints

- **NO DYNAMIC MEMORY IN RUNTIME PATHS:**
  - Avoid `malloc`/`free` in real-time or control paths
  - Dynamic allocation is allowed during initialization if justified
  - Prefer static allocation or memory pools

- **MINIMAL STANDARD LIBRARIES:**
  - Avoid `stdio.h`, `stdlib.h` unless clearly justified
  - Prefer ESP-IDF logging instead of printf

- **SAFETY FIRST:**
  - Prefer MISRA C:2012 compliant code
  - Deviations allowed when:
    - required for ESP-IDF APIs
    - required for performance or system constraints
  - Every deviation must include justification

- **TARGET:**
  - MCU: ESP32 (Xtensa dual-core)
  - Framework: ESP-IDF (default)
  - OS: FreeRTOS (built-in)
  - Language: C

---

## 2. ESP-IDF / Project Rules

- Follow ESP-IDF component structure:
  - each module should be its own component when appropriate

- Do NOT:
  - bypass ESP-IDF drivers unless justified
  - mix application logic with driver initialization

- Prefer:
  - `esp_err_t` return types
  - `ESP_ERROR_CHECK()` for critical calls

- Logging:
  - Use `ESP_LOGI/W/E/D`
  - Avoid printf in production

---

## 3. Coding Style

- Use `stdint.h` types (`uint32_t`, etc.)
- Avoid magic numbers → define constants in CAPS

- Use `volatile` ONLY for:
  - memory-mapped registers
  - ISR-shared variables

- Code should be:
  - deterministic where required
  - readable
  - modular

- Avoid:
  - recursion
  - hidden side effects
  - blocking delays (`vTaskDelay` is OK when intentional)

---

## 4. Rule of Four (Guideline)

This is a guideline, not strict. Clarity wins.

1. Function names: ≤4 words + one verb  
2. Max ~4 arguments  
3. Function length ~25 lines  
4. Four abstraction levels:

- Level 1: plain English
- Level 2: pseudo code
- Level 3: generic C logic
- Level 4: ESP-IDF / hardware specific

---

## 5. Real-Time & FreeRTOS Rules

ESP32 is RTOS-based — concurrency is critical.

### Tasks
- Keep tasks focused and small
- Avoid long blocking operations unless intentional
- Define stack size explicitly
- Assign priorities carefully

### ISRs
- Must be:
  - minimal
  - non-blocking
- Use:
  - queues
  - semaphores
  - event groups
- NEVER call blocking FreeRTOS APIs inside ISR

### Synchronization
- Protect shared data:
  - mutex
  - critical sections
- Avoid race conditions

### Timing
When modifying:
- task scheduling
- ISR behavior
- timers
- communication stacks

Claude MUST:
- explain timing assumptions
- identify concurrency risks
- mention task priority interactions

---

## 6. SOLID Principles (Embedded Adaptation)

### SRP
- One module per responsibility
  - e.g., `wifi_manager.c`, `motor_control.c`, `uart_driver.c`

### OCP
- Extend via function pointers or config structs

### LSP
- Interface-based drivers must be interchangeable

### ISP
- Keep APIs small and focused

### DIP
- High-level logic must not depend on ESP-IDF directly
- Inject dependencies via interfaces

### Example
- `wifi_driver.c` → hardware/network
- `app_logic.c` → uses interface

---

## 7. Testing

- Use host-side tests when possible
- Design for testability:
  - dependency injection
  - modular interfaces

- Claude should:
  - generate testable code
  - suggest test strategy
  - provide example tests if relevant

---

## 8. Documentation

Use Doxygen format:

- File headers
- Function docs
- Structs and enums

Document:
- assumptions
- timing
- units
- concurrency constraints

Explain WHY, not WHAT

---

## 9. Output Format Requirements

When making changes, Claude MUST include:

### Build Results (if applicable)
- text: X bytes
- data: Y bytes
- bss: Z bytes

If not applicable:
- "Build results: N/A"

---

### Implementation Summary

| File | Changes |
|------|--------|
| path/to/file.c | Description |
| path/to/file.h | Description |

---

### Protocol/API Summary (if applicable)

Document:
- APIs
- message formats
- communication protocols (UART, WiFi, BLE, etc.)

If not applicable:
- "Protocol/API: N/A"

---

## 10. Response Expectations

When proposing changes:

1. What changed
2. Why it changed
3. Risks / assumptions
4. How to validate

For bugs:
- explain root cause first

For refactors:
- preserve behavior unless told otherwise

---

## 11. Repository Exploration

Before major changes, inspect:
- component structure
- sdkconfig
- FreeRTOS usage
- task creation
- interrupt usage
- communication stacks (WiFi/BLE/UART)

Follow existing patterns unless harmful

---

## 12. When Uncertain

- Do NOT assume hardware details
- State assumptions clearly
- Ask questions if needed

If rules conflict:
1. follow repo
2. explain conflict
3. choose safest solution